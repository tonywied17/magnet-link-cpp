/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\MagnetMetadata.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 9:09:29 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef MAGNET_METADATA_H
#define MAGNET_METADATA_H

#include <string>
#include <vector>
#include <cstdint>

struct DHTNode
{
    std::string ip;
    uint16_t port;

    DHTNode(std::string ip, uint16_t port) : ip(std::move(ip)), port(port) {}
};

class MagnetMetadata
{
public:
    /*!
        \brief Creates a MagnetMetadata object with the given hash, trackers, piece hashes, and piece size.
        \param hash The SHA-1 hash of the torrent.
        \param trackers The list of tracker URLs.
        \param pieceHashes The list of piece hashes (one per piece).
        \param pieceSize The size of each piece.
        \param displayName The display name of the torrent.
        \param nodes The list of DHT nodes.
    */
    MagnetMetadata(std::string hash, std::vector<std::string> trackers,
                   std::vector<std::string> pieceHashes, uint32_t pieceSize, std::string displayName,
                   std::vector<DHTNode> nodes, uint32_t port);

    /*!
        \brief Returns the SHA-1 hash of the torrent.
        \return The SHA-1 hash.
    */
    const std::string &getInfoHash() const;

    /*!
        \brief Returns the list of tracker URLs.
        \return The list of tracker URLs.
    */
    const std::vector<std::string> &getTrackers() const;

    /*!
        \brief Returns the list of piece hashes (one per piece).
        \return The list of piece hashes.
    */
    const std::vector<std::string> &getPieceHashes() const;

    /*!
        \brief Returns the size of each piece.
        \return The size of each piece.
    */
    uint32_t getPieceSize() const;

    /*!
        \brief Returns the display name of the torrent.
        \return The display name.
    */
    const std::string &getDisplayName() const;

    /*!
        \brief Returns the list of DHT nodes.
        \return The list of DHT nodes.
    */
    const std::vector<DHTNode> &getNodes() const;

    /*!
        \brief Returns the DHT port.
        \return The DHT port.
    */
    uint32_t getPort() const;

    void setPieceHashes(const std::vector<std::string> &pieceHashes);

    void setNodes(const std::vector<DHTNode> &nodes);

private:
    std::string infoHash;                 //!> SHA-1 hash
    std::vector<std::string> trackers;    //!> List of tracker URLs
    std::vector<std::string> pieceHashes; //!> List of piece hashes (one per piece)
    uint32_t pieceSize;                   //!> Size of each piece
    std::string displayName;              //!> Display name (torrent name)
    std::vector<DHTNode> nodes;           //!> DHT nodes (peer list)
    uint32_t port;                        //!> DHT port (default: 6881)
};

#endif
