/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\DHTClient.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 4:37:05 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef DHT_CLIENT_H
#define DHT_CLIENT_H

#include <string>
#include <vector>
#include <array>
#include <cstdint>

class DHTClient
{
public:
    /*!
        \brief Constructor for DHTClient.
        \param infoHash The info hash of the torrent file.
    */
    explicit DHTClient(const std::string &infoHash);

    /*!
        \brief Destructor for DHTClient.
    */
    ~DHTClient();

    /*!
        \brief Get a list of peers for the torrent file.
        \return A vector of strings containing peer information.
    */
    std::vector<std::string> getPeers();

    /*!
        \brief Get a list of peers for the torrent file from multiple bootstrap nodes.
        \return A vector of strings containing peer information.
    */
    std::string buildGetPeersQuery();

    /*!
        \brief Fetch peers from a specific DHT node with a custom query.
        \param node The DHT node address.
        \param query The get_peers query to send to the node.
        \return A vector of strings containing peer information.
    */
    std::vector<std::string> getPeersFromNode(const std::string &node, const std::string &query);

private:
    std::string infoHash;                                                //!> The info hash of the torrent file.
    int createSocket();                                                  //!> Creates a UDP socket for DHT communication.
    std::vector<std::string> parseResponse(const std::string &response); //!> Parses DHT response to extract peer info.
    std::array<uint8_t, 20> generateNodeID();                            //!> Generates a random 20-byte node ID
};

#endif
