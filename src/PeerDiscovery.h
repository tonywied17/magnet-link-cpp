/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\torrent_library\PeerDiscovery.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\torrent_library
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 1:44:04 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef PEER_DISCOVERY_H
#define PEER_DISCOVERY_H

#include <vector>
#include <string>
#include <cstdint>
#include "DHTClient.h"

class PeerDiscovery
{
public:
    /*!
        \brief Creates a PeerDiscovery object with the given info hash.
        \param infoHash The info hash of the torrent.
    */
    explicit PeerDiscovery(const std::string &infoHash);

    /*!
        \brief Discovers peers for the torrent.
        \return A vector of peer addresses.
    */
    std::vector<std::string> discoverPeers();

private:
    std::string infoHash; //!< Info hash of the torrent file.
    DHTClient dhtClient;  //!< DHT client.
};

#endif
