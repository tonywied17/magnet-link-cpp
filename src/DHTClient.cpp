/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\DHTClient.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 4:37:11 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

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
    \brief Generates a random 20-byte node ID.
    \return A 20-byte array containing the node ID.
*/
std::array<uint8_t, 20> DHTClient::generateNodeID()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::array<uint8_t, 20> nodeID;
    for (uint8_t &byte : nodeID)
    {
        byte = static_cast<uint8_t>(dis(gen));
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
    std::string nodeIDStr;

    for (auto byte : nodeID)
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        nodeIDStr += ss.str();
    }

    std::cout << "Node ID (Hex): " << nodeIDStr << std::endl;

    std::unordered_map<std::string, std::string> args;
    args["id"] = nodeIDStr;
    args["info_hash"] = infoHash;

    std::unordered_map<std::string, std::string> query;
    query["t"] = "aa";                                       //!> Transaction ID
    query["y"] = "q";                                        //!> Query type
    query["q"] = "get_peers";                                //!> Query method
    query["a"] = TorrentUtilities::encodeBencodedData(args); //!> Encoded arguments

    std::string fullQuery = TorrentUtilities::encodeBencodedData(query);

    std::cout << "Full Query: " << fullQuery << std::endl;

    return fullQuery;
}

/*!
    \brief Get a list of peers for the torrent file from multiple bootstrap nodes.
    \return A vector of strings containing peer information.
*/
std::vector<std::string> DHTClient::getPeers()
{
    std::vector<std::string> allPeers;
    std::vector<std::string> bootstrapNodes = {
        "67.215.246.10",
        "router.bittorrent.com",
        "dht.transmissionbt.com"};

    std::cout << "Starting to discover peers..." << std::endl;

    std::string query = buildGetPeersQuery();

    for (const auto &node : bootstrapNodes)
    {
        std::cout << "Attempting to fetch peers from node: " << node << std::endl;

        try
        {
            std::vector<std::string> peers = getPeersFromNode(node, query);
            std::cout << "Received " << peers.size() << " peers from node: " << node << std::endl;
            allPeers.insert(allPeers.end(), peers.begin(), peers.end());
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Error fetching peers from node " << node << ": " << e.what() << std::endl;
        }
    }

    std::cout << "Total peers discovered: " << allPeers.size() << std::endl;

    return allPeers;
}

/*!
    \brief Get a list of peers from a specific DHT node.
    \param node The DHT node address.
    \param query The get_peers query to send to the node.
    \return A vector of strings containing peer information.
*/
std::vector<std::string> DHTClient::getPeersFromNode(const std::string &node, const std::string &query)
{
    int sock = createSocket();

    struct sockaddr_in dhtNode{};
    dhtNode.sin_family = AF_INET;
    dhtNode.sin_port = htons(DHT_PORT);

    if (inet_pton(AF_INET, node.c_str(), &dhtNode.sin_addr) <= 0)
    {
#ifdef _WIN32
        if (closesocket(sock) != 0)
        {
            std::cerr << "Failed to close socket" << std::endl;
        }
#else
        if (close(sock) < 0)
        {
            std::cerr << "Failed to close socket" << std::endl;
        }
#endif
        throw std::runtime_error("Invalid DHT node address");
    }

    if (sendto(sock, query.c_str(), query.size(), 0, (struct sockaddr *)&dhtNode, sizeof(dhtNode)) < 0)
    {
#ifdef _WIN32
        if (closesocket(sock) != 0)
        {
            std::cerr << "Failed to close socket" << std::endl;
        }
#else
        if (close(sock) < 0)
        {
            std::cerr << "Failed to close socket" << std::endl;
        }
#endif
        throw std::runtime_error("Failed to send get_peers request");
    }

    char buffer[BUFFER_SIZE];
    socklen_t addrLen = sizeof(dhtNode);
    int recvLen = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&dhtNode, &addrLen);

    std::vector<std::string> peers;
    if (recvLen > 0)
    {
        std::string response(buffer, recvLen);
        peers = parseResponse(response);
    }

#ifdef _WIN32
    if (closesocket(sock) != 0)
    {
        std::cerr << "Failed to close socket" << std::endl;
    }
#else
    if (close(sock) < 0)
    {
        std::cerr << "Failed to close socket" << std::endl;
    }
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
    if (decoded.find("r") == decoded.end())
    {
        return peers;
    }

    size_t index = 0;
    auto result = TorrentUtilities::decodeBencodedData(decoded["r"], index);

    if (result.find("values") != result.end())
    {
        auto peerList = TorrentUtilities::decodeBencodedList(result["values"], index);

        for (const std::string &peerData : peerList)
        {
            if (peerData.size() != 6)
            {
                continue;
            }

            uint8_t ip1 = static_cast<uint8_t>(peerData[0]);
            uint8_t ip2 = static_cast<uint8_t>(peerData[1]);
            uint8_t ip3 = static_cast<uint8_t>(peerData[2]);
            uint8_t ip4 = static_cast<uint8_t>(peerData[3]);
            uint16_t port = (static_cast<uint8_t>(peerData[4]) << 8) | static_cast<uint8_t>(peerData[5]);

            std::ostringstream peerAddress;
            peerAddress << static_cast<int>(ip1) << "."
                        << static_cast<int>(ip2) << "."
                        << static_cast<int>(ip3) << "."
                        << static_cast<int>(ip4) << ":"
                        << port;

            peers.push_back(peerAddress.str());
        }
    }

    return peers;
}
