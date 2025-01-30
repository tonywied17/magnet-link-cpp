/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\PeerConnection.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 4:36:50 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef PEER_CONNECTION_H
#define PEER_CONNECTION_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include <string>
#include <vector>
#include <cstdint>
#include "MagnetMetadata.h"

class PeerConnection
{
public:
    /*!
        \brief Creates a PeerConnection object with the given peer address and metadata.
        \param peerAddress The IP and port of the peer.
        \param metadata The metadata of the torrent.
    */
    explicit PeerConnection(const std::string &peerAddress, const MagnetMetadata &metadata);
    ~PeerConnection();

    bool connectToPeer();                                                              //!> Initiates the connection to the peer.
    void performHandshake();                                                           //!> Perform the torrent protocol handshake with the peer.
    void sendRequest(uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength); //!> Request data (torrent pieces) from the peer.
    std::vector<char> receiveData();                                                   //!> Receive data from the peer (torrent pieces).

private:
    std::string peerAddress; //!> Peer IP and port.
    std::string infoHash;    //!> Info hash of the torrent (for handshake).
    SOCKET socketFd;         //!> Socket descriptor for the peer connection.
    

    void createSocket();
    void setSocketTimeout(int timeout);
    //!> Create a socket for the peer connection.
    void closeSocket();  //!> Close the socket for the peer connection.
};

#endif