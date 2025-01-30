/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\torrent_library\MagnetMetadata.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 4:08:57 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */
#ifndef MAGNET_METADATA_H
#define MAGNET_METADATA_H

#include <string>
#include <vector>
#include <cstdint>

class MagnetMetadata
{
public:
    /*!
        \brief Creates a MagnetMetadata object with the given hash, trackers, piece hashes, and piece size.
        \param hash The SHA-1 hash of the torrent.
        \param trackers The list of tracker URLs.
        \param pieceHashes The list of piece hashes (one per piece).
        \param pieceSize The size of each piece.
    */
    MagnetMetadata(std::string hash, std::vector<std::string> trackers,
                   std::vector<std::string> pieceHashes, uint32_t pieceSize);

    /*!
        \brief Get the SHA-1 hash of the torrent.
        \return The SHA-1 hash.
    */
    const std::string &getInfoHash() const;

    /*!
        \brief Get the list of tracker URLs.
        \return The list of tracker URLs.
    */
    const std::vector<std::string> &getTrackers() const;

    /*!
        \brief Get the list of piece hashes.
        \return The list of piece hashes.
    */
    const std::vector<std::string> &getPieceHashes() const;

    /*!
        \brief Get the size of each piece.
        \return The size of each piece.
    */
    uint32_t getPieceSize() const;

private:
    std::string infoHash;                 //!> SHA-1 hash
    std::vector<std::string> trackers;    //!> List of tracker URLs
    std::vector<std::string> pieceHashes; //!> List of piece hashes (one per piece)
    uint32_t pieceSize;                   //!> Size of each piece
};

#endif
