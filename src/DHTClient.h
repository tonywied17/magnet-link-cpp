/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\DHTClient.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 8:56:13 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef DHTCLIENT_H
#define DHTCLIENT_H

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <system_error>
#include <functional>
#include "MagnetMetadata.h"

class DHTClient
{
public:
    /*!
        \brief DHTClient constructor
        \param infoHash The info hash of the torrent
        \param port The port to use for the torrent
        \param nodes The list of DHT nodes for the torrent
    */
    DHTClient(const std::string &infoHash, uint16_t port);

    /*!
        \brief Discover peers for the torrent
        \param callback The callback to call when peers are discovered
    */
    void discoverPeers();

private:
    /*!
        \brief Send a request to a DHT node
        \param node The DHT node to send the request to
        \param infoHash The info hash of the torrent
    */
    void sendDHTRequest(const std::string &node, const std::string &infoHash);

    /*!
        \brief Process the response from a DHT node
        \param response The response from the DHT node
    */
    void processDHTResponse(const std::string &response);

    /*!
        \brief Handle peer discovery
        \param ec The error code
        \param bytes_transferred The number of bytes transferred
        \param endpoint The endpoint of the DHT node
    */
    void handlePeerDiscovery(const boost::system::error_code &ec, size_t bytes_transferred,
                             const boost::asio::ip::udp::endpoint &endpoint);

    /*!
        \brief Extract the peers from the response
        \param nodesData The response from the DHT node
        \return The list of peers
    */
    std::vector<std::string> extractPeers(const std::string &nodesData);

    char responseBuffer[1024];               //!> Buffer to store the response from the DHT node
    std::string infoHash;                    //!> The info hash of the torrent
    uint16_t port;                           //!> The port to use for the torrent
    boost::asio::io_context io_context;      //!> The I/O context
    boost::asio::ip::udp::socket socket;     //!> The UDP socket
    boost::asio::ip::udp::endpoint endpoint; //!> The endpoint for the DHT node
    std::unique_ptr<MagnetMetadata> metadata; //!> The metadata of the torrent
    std::vector<std::string> nodes;          //!> The list of DHT nodes for the torrent
};

#endif
