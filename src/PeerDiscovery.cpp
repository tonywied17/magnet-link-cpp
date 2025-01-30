/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\PeerDiscovery.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 4:36:27 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */


#include "PeerDiscovery.h"
#include <stdexcept>

/*!
    \brief Creates a PeerDiscovery object with the given info hash.
    \param infoHash The info hash of the torrent.
*/
PeerDiscovery::PeerDiscovery(const std::string &infoHash) : infoHash(infoHash), dhtClient(infoHash) {}

/*!
    \brief Discovers peers for the torrent.
    \return A vector of peer addresses.
*/
std::vector<std::string> PeerDiscovery::discoverPeers()
{
    std::vector<std::string> allPeers;
    try
    {
        DHTClient dhtClient(infoHash);
        std::vector<std::string> nodes = dhtClient.getPeers();
        for (const auto &node : nodes)
        {
            std::string query = dhtClient.buildGetPeersQuery();
            std::vector<std::string> peersFromNode = dhtClient.getPeersFromNode(node, query);
            allPeers.insert(allPeers.end(), peersFromNode.begin(), peersFromNode.end());
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Failed to discover peers: " + std::string(e.what()));
    }
    return allPeers;
}