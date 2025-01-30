/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\torrent_library\DownloadTorrent.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\torrent_library
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 1:43:35 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef DOWNLOAD_TORRENT_H
#define DOWNLOAD_TORRENT_H

#include <string>
#include <vector>
#include <fstream>
#include "PeerConnection.h"
#include "MagnetMetadata.h"

class DownloadTorrent
{
public:
    /*!
        \brief Creates a DownloadTorrent object with the given metadata.
        \param metadata The metadata of the torrent.
    */
    explicit DownloadTorrent(const MagnetMetadata &metadata);

    /*!
        \brief Starts the download process.
    */
    void startDownload();

private:
    MagnetMetadata metadata;        //!> The metadata of the torrent.
    std::vector<std::string> peers; //!> The list of peers to connect to.
    std::string downloadDirectory;  //!> The directory to save the downloaded files.

    void requestPieces();                                                      //!> Request pieces from peers.
    void savePiece(uint32_t pieceIndex, const std::vector<char> &pieceData);   //!> Save the downloaded piece.
    void createDownloadDirectory();                                            //!> Ensure the download directory exists.
    void updatePieceStatus(uint32_t pieceIndex, bool isDownloaded);            //!> Track the status of pieces.
    bool verifyPiece(uint32_t pieceIndex, const std::vector<char> &pieceData); //!> Verify piece integrity.
    void retryPieceDownload(uint32_t pieceIndex);                              //!> Retry downloading a piece if it fails.
    std::string calculateSHA1(const std::vector<char> &data);                  //!> Calculate the SHA-1 hash of the piece data.
};

#endif