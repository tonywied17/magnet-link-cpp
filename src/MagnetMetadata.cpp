/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\MagnetMetadata.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 3:51:07 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "MagnetMetadata.h"
#include <cstdint>
#include <iostream>

/*!
    \brief Creates a MagnetMetadata object with the given hash, trackers, piece hashes, and piece size.
    \param hash The SHA-1 hash of the torrent.
    \param trackers The list of tracker URLs.
    \param pieceHashes The list of piece hashes (one per piece).
    \param pieceSize The size of each piece.
*/
MagnetMetadata::MagnetMetadata(std::string hash, std::vector<std::string> trackers,
                               std::vector<std::string> pieceHashes, uint32_t pieceSize)
    : infoHash(std::move(hash)), trackers(std::move(trackers)),
      pieceHashes(std::move(pieceHashes)), pieceSize(pieceSize) {}

/*!
    \brief Get the info hash
*/
const std::string &MagnetMetadata::getInfoHash() const
{
    std::cout << "Info hash: " << infoHash << std::endl;
    return infoHash;
}

/*!
    \brief Get the tracker URLs
*/
const std::vector<std::string> &MagnetMetadata::getTrackers() const
{
    return trackers;
}

/*!
    \brief Get the list of piece hashes
*/
const std::vector<std::string> &MagnetMetadata::getPieceHashes() const
{
    std::cout << "Piece hashes: " << std::endl;
    return pieceHashes;
}

/*!
    \brief Get the size of each piece
*/
uint32_t MagnetMetadata::getPieceSize() const
{
    std::cout << "Piece size: " << pieceSize << std::endl;
    return pieceSize;
}
