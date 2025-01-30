#include "DHTClient.h"
#include "TorrentUtilities.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <random>
#include <array>
#include <unordered_map>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <cstdint>
#include <iomanip>

#define DHT_PORT 6881
#define BUFFER_SIZE 1024

/*!
    \brief Constructor for DHTClient.
    \param infoHash The info hash of the torrent file.
*/
DHTClient::DHTClient(const std::string &infoHash) : infoHash(infoHash)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw std::runtime_error("WSAStartup failed");
    }
#endif
}

/*!
    \brief Destructor for DHTClient.
*/
DHTClient::~DHTClient()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

/*!
    \brief Creates a UDP socket for DHT communication.
    \return The socket file descriptor.
*/
int DHTClient::createSocket()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        throw std::runtime_error("Failed to create UDP socket");
    }
    return sock;
}

/*!
    \brief Generates a random 20-byte node ID
    \return A 20-byte array containing the node ID.
*/
std::array<uint8_t, 20> DHTClient::generateNodeID()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::array<uint8_t, 20> nodeID;
    for (size_t i = 0; i < 20; ++i)
    {
        nodeID[i] = dis(gen);
    }

    return nodeID;
}

/*!
    \brief Builds a get_peers query to send to DHT nodes.
    \return The bencoded query string.
*/
std::string DHTClient::buildGetPeersQuery()
{
    std::array<uint8_t, 20> nodeID = generateNodeID();

    std::stringstream nodeIDStream;
    for (auto byte : nodeID)
    {
        nodeIDStream << std::setw(2) << std::setfill('0') << std::hex << (int)byte;
    }
    std::string nodeIDStr = nodeIDStream.str(); //* to hex string

    //* DHT messages- encode a "get_peers" request.
    std::unordered_map<std::string, std::string> query;
    query["t"] = "aa";        // transaction ID
    query["y"] = "q";         // query type
    query["q"] = "get_peers"; // query method

    std::unordered_map<std::string, std::string> args;
    args["id"] = nodeIDStr;
    args["info_hash"] = infoHash; //* Target torrent hash

    query["a"] = TorrentUtilities::encodeBencodedData(args);
    return TorrentUtilities::encodeBencodedData(query);
}

/*!
    \brief Get a list of peers for the torrent file.
    \return A vector of strings containing peer information.
*/
std::vector<std::string> DHTClient::getPeers()
{
    std::vector<std::string> peers;
    int sock = createSocket();

    struct sockaddr_in dhtNode;
    dhtNode.sin_family = AF_INET;
    dhtNode.sin_port = htons(DHT_PORT);

    if (inet_pton(AF_INET, "67.215.246.10", &dhtNode.sin_addr) <= 0)
    { //! Public DHT bootstrap node
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        throw std::runtime_error("Invalid DHT bootstrap node address");
    }

    std::string query = buildGetPeersQuery();
    sendto(sock, query.c_str(), query.size(), 0, (struct sockaddr *)&dhtNode, sizeof(dhtNode));

    char buffer[BUFFER_SIZE];
    socklen_t addrLen = sizeof(dhtNode);
    int recvLen = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&dhtNode, &addrLen);

    if (recvLen > 0)
    {
        std::string response(buffer, recvLen);
        peers = parseResponse(response);
    }

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    return peers;
}

/*!
    \brief Parses DHT response to extract peer info.
    \param response The response from the DHT node.
    \return A vector of strings containing peer information.
*/
std::vector<std::string> DHTClient::parseResponse(const std::string &response)
{
    std::vector<std::string> peers;

    auto decoded = TorrentUtilities::decodeBencodedData(response);
    if (decoded.find("r") != decoded.end())
    {
        auto result = decoded["r"];

        if (result.find("values") != std::string::npos)
        {
            std::string peerData = decoded["values"];

            if (peerData.size() % 6 != 0)
            {
                throw std::runtime_error("Invalid peer data length");
            }

            for (size_t i = 0; i < peerData.size(); i += 6)
            {
                uint8_t ip1 = static_cast<uint8_t>(peerData[i]);
                uint8_t ip2 = static_cast<uint8_t>(peerData[i + 1]);
                uint8_t ip3 = static_cast<uint8_t>(peerData[i + 2]);
                uint8_t ip4 = static_cast<uint8_t>(peerData[i + 3]);
                uint16_t port = (static_cast<uint8_t>(peerData[i + 4]) << 8) | static_cast<uint8_t>(peerData[i + 5]);

                std::ostringstream peerAddress;
                peerAddress << static_cast<int>(ip1) << "."
                            << static_cast<int>(ip2) << "."
                            << static_cast<int>(ip3) << "."
                            << static_cast<int>(ip4) << ":"
                            << port;

                peers.push_back(peerAddress.str());
            }
        }
    }

    return peers;
}