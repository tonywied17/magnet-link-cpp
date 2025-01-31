/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\MagnetMetadata.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 8:17:45 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "MagnetMetadata.h"
#include <cstdint>
#include <iostream>

/*!
    \brief MagnetMetadata constructor
    \param hash The info hash of the torrent
    \param trackers The list of trackers for the torrent
    \param pieceHashes The list of piece hashes for the torrent
    \param pieceSize The size of each piece in bytes
    \param displayName The display name of the torrent
    \param nodes The list of DHT nodes for the torrent
    \param port The port to use for the torrent
*/
MagnetMetadata::MagnetMetadata(std::string hash, std::vector<std::string> trackers,
                               std::vector<std::string> pieceHashes, uint32_t pieceSize, std::string displayName,
                               std::vector<DHTNode> nodes, uint32_t port)
    : infoHash(std::move(hash)), trackers(std::move(trackers)),
      pieceHashes(std::move(pieceHashes)), pieceSize(pieceSize), displayName(std::move(displayName)),
      nodes(std::move(nodes)), port(port) {}

const std::string &MagnetMetadata::getInfoHash() const
{
    return infoHash;
}

const std::vector<std::string> &MagnetMetadata::getTrackers() const
{
    return trackers;
}

const std::vector<std::string> &MagnetMetadata::getPieceHashes() const
{
    return pieceHashes;
}

uint32_t MagnetMetadata::getPieceSize() const
{
    return pieceSize;
}

const std::string &MagnetMetadata::getDisplayName() const
{
    return displayName;
}

const std::vector<DHTNode> &MagnetMetadata::getNodes() const
{
    return nodes;
}

uint32_t MagnetMetadata::getPort() const
{
    return port;
}
