/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\DHTClient.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 9:08:18 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "DHTClient.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <boost/asio.hpp>
#include <functional>
#include "TorrentUtilities.h"
#include "MagnetMetadata.h"

/*!
    \brief DHTClient constructor
    \param infoHash The info hash of the torrent
    \param port The port to use for the torrent
    \param nodes The list of DHT nodes for the torrent
*/
DHTClient::DHTClient(const std::string &infoHash, uint16_t port)
    : infoHash(infoHash), port(port),
      socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {}

/*!
    \brief Discover peers for the torrent
    \param callback The callback to call when peers are discovered
*/
void DHTClient::discoverPeers()
{
    if (metadata && !metadata->getNodes().empty())
    {
        for (const auto &node : metadata->getNodes())
        {
            sendDHTRequest(node.ip, infoHash);
        }
    }
    else
    {
        std::vector<std::string> defaultNodes = {
            "dht.transmissionbt.com", "router.bittorrent.com", "dht.aelitis.com"};

        for (const auto &node : defaultNodes)
        {
            sendDHTRequest(node, infoHash);
        }
    }

    std::cout << "Running IO context to handle responses..." << std::endl;
    std::thread([this]()
                { io_context.run(); })
        .detach();

    std::this_thread::sleep_for(std::chrono::seconds(5));
}

/*!
    \brief Send a request to a DHT node
    \param node The DHT node to send the request to
    \param infoHash The info hash of the torrent
*/
void DHTClient::sendDHTRequest(const std::string &node, const std::string &infoHash)
{
    std::string message = "DHT Request: InfoHash=" + infoHash;

    try
    {
        boost::asio::ip::udp::resolver resolver(io_context);
        boost::asio::ip::udp::resolver::results_type endpoints = resolver.resolve(node, "6881");

        for (const auto &endpoint : endpoints)
        {
            socket.send_to(boost::asio::buffer(message), endpoint);
            std::cout << "Sent DHT request to " << endpoint.endpoint() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error resolving " << node << ": " << e.what() << std::endl;
    }
}

/*!
    \brief Handle peer discovery
    \param ec The error code
    \param bytes_transferred The number of bytes transferred
    \param endpoint The endpoint of the DHT node
*/
void DHTClient::handlePeerDiscovery(const boost::system::error_code &ec, size_t bytes_transferred,
                                    const boost::asio::ip::udp::endpoint &endpoint)
{
    if (!ec)
    {
        std::string response_data(responseBuffer, bytes_transferred);
        std::cout << "Received response from: " << endpoint.address().to_string() << std::endl;

        processDHTResponse(response_data);
    }
    else
    {
        std::cerr << "Error receiving response: " << ec.message() << std::endl;
    }
}

/*!
    \brief Process the response from a DHT node
    \param response The response from the DHT node
*/
void DHTClient::processDHTResponse(const std::string &response)
{
    std::cout << "Received DHT response: " << response << std::endl;

    size_t index = 0;

    try
    {
        auto decodedData = TorrentUtilities::decodeBencodedData(response, index);

        std::cout << "Decoded DHT response: " << TorrentUtilities::toString(decodedData) << std::endl;

        if (decodedData.find("nodes") != decodedData.end())
        {
            std::string nodesData = decodedData["nodes"];
            std::vector<std::string> peers = extractPeers(nodesData);

            metadata->setNodes(std::vector<DHTNode>());
            std::cout << "Updated existing MagnetMetadata object with new DHT nodes." << std::endl;

            for (const auto &peer : peers)
            {
                std::cout << "Found peer: " << peer << std::endl;
            }
        }

        if (decodedData.find("pieces") != decodedData.end())
        {
            std::string piecesData = decodedData["pieces"];
            std::vector<std::string> pieceHashes;

            for (size_t i = 0; i < piecesData.size(); i += 20)
            {
                pieceHashes.push_back(piecesData.substr(i, 20));
            }

            metadata->setPieceHashes(pieceHashes);
            std::cout << "Updated existing MagnetMetadata object with new piece hashes." << std::endl;

            if (metadata->getPieceSize() > 0)
            {
                std::cout << "Torrent is complete." << std::endl;
            }
        }
        else
        {
            std::cerr << "No pieces found in the DHT response." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing DHT response: " << e.what() << std::endl;
    }
}

/*!
    \brief Extract the peers from the response
    \param nodesData The response from the DHT node
    \return The list of peers
*/
std::vector<std::string> DHTClient::extractPeers(const std::string &nodesData)
{
    std::vector<std::string> peers;

    if (nodesData.size() % 6 != 0)
    {
        std::cerr << "Invalid node data size: " << nodesData.size() << std::endl;
        return peers;
    }

    for (size_t i = 0; i < nodesData.size(); i += 6)
    {
        uint8_t ip1 = static_cast<uint8_t>(nodesData[i]);
        uint8_t ip2 = static_cast<uint8_t>(nodesData[i + 1]);
        uint8_t ip3 = static_cast<uint8_t>(nodesData[i + 2]);
        uint8_t ip4 = static_cast<uint8_t>(nodesData[i + 3]);

        uint16_t port = (static_cast<uint8_t>(nodesData[i + 4]) << 8) |
                        static_cast<uint8_t>(nodesData[i + 5]);

        std::string peer = std::to_string(ip1) + "." +
                           std::to_string(ip2) + "." +
                           std::to_string(ip3) + "." +
                           std::to_string(ip4) + ":" +
                           std::to_string(port);

        peers.push_back(peer);
    }

    return peers;
}
