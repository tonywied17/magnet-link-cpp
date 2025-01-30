/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\PeerConnection.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 4:38:56 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "PeerConnection.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif

/*!
    \brief Creates a PeerConnection object with the given peer address and metadata.
    \param peerAddress The IP and port of the peer.
    \param metadata The metadata of the torrent.
*/
PeerConnection::PeerConnection(const std::string &peerAddress, const MagnetMetadata &metadata)
    : peerAddress(peerAddress), infoHash(metadata.getInfoHash()), socketFd(INVALID_SOCKET) {}

/*!
    \brief Destroys the PeerConnection object.
*/
PeerConnection::~PeerConnection()
{
    closeSocket();
}

/*!
    \brief Creates and configures the socket.
*/
void PeerConnection::createSocket()
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw std::runtime_error("Winsock initialization failed");
    }

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == INVALID_SOCKET)
    {
        WSACleanup();
        throw std::runtime_error("Failed to create socket");
    }
#else
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }
#endif
}

/*!
    \brief Sets a socket timeout for blocking operations.
    \param timeout The timeout value in seconds.
*/
void PeerConnection::setSocketTimeout(int timeout)
{
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;

#ifdef _WIN32
    setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
    setsockopt(socketFd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv));
#else
    setsockopt(socketFd, SOL_SOCKET, SO_RCVBUF, &tv, sizeof(tv));
    setsockopt(socketFd, SOL_SOCKET, SO_RCVBUF, &tv, sizeof(tv));
#endif
}

/*!
    \brief Closes the socket connection.
*/
void PeerConnection::closeSocket()
{
#ifdef _WIN32
    if (socketFd != INVALID_SOCKET)
    {
        closesocket(socketFd);
        WSACleanup();
        socketFd = INVALID_SOCKET;
    }
#else
    if (socketFd >= 0)
    {
        close(socketFd);
        socketFd = -1;
    }
#endif
}

/*!
    \brief Initiates the connection to the peer.
    \throws std::runtime_error if connection fails.
*/
bool PeerConnection::connectToPeer()
{
    size_t colonPos = peerAddress.find(':');
    if (colonPos == std::string::npos)
    {
        throw std::runtime_error("Invalid peer address format");
    }

    std::string ip = peerAddress.substr(0, colonPos);
    int port = std::stoi(peerAddress.substr(colonPos + 1));

    createSocket();

    struct sockaddr_in peerAddr;
    peerAddr.sin_family = AF_INET;
    peerAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &peerAddr.sin_addr) <= 0)
    {
        closeSocket();
        throw std::runtime_error("Invalid peer IP address");
    }

    setSocketTimeout(10); // 10-second timeout for connection

#ifdef _WIN32
    if (connect(socketFd, (struct sockaddr *)&peerAddr, sizeof(peerAddr)) == SOCKET_ERROR)
    {
        closeSocket();
        throw std::runtime_error("Failed to connect to peer");
    }
#else
    if (connect(socketFd, (struct sockaddr *)&peerAddr, sizeof(peerAddr)) < 0)
    {
        closeSocket();
        throw std::runtime_error("Failed to connect to peer");
    }
#endif

    return true;
}

/*!
    \brief Perform the torrent protocol handshake with the peer.
    \throws std::runtime_error if handshake fails.
*/
void PeerConnection::performHandshake()
{
    char handshake[68] = {0};
    handshake[0] = 19;                                     //!> Protocol length - 19 for bittorrent protocol
    std::memcpy(handshake + 1, "BitTorrent protocol", 19); //!> Protocol string
    std::memcpy(handshake + 28, infoHash.c_str(), 20);     //!> Info hash
    std::memcpy(handshake + 48, "\0\0\0\0\0\0\0\0", 8);    //!> Reserved bytes

#ifdef _WIN32
    if (send(socketFd, handshake, sizeof(handshake), 0) == SOCKET_ERROR)
    {
        throw std::runtime_error("Failed to send handshake");
    }

    char response[68];
    if (recv(socketFd, response, sizeof(response), 0) == SOCKET_ERROR)
    {
        throw std::runtime_error("Failed to receive handshake");
    }
#else
    if (send(socketFd, handshake, sizeof(handshake), 0) < 0)
    {
        throw std::runtime_error("Failed to send handshake");
    }

    char response[68];
    if (recv(socketFd, response, sizeof(response), 0) < 0)
    {
        throw std::runtime_error("Failed to receive handshake");
    }
#endif

    std::cout << "Handshake successful with peer!" << std::endl;
}

/*!
    \brief Request data (torrent pieces) from the peer.
    \param pieceIndex The index of the piece to request.
    \param blockOffset The offset of the block within the piece.
    \param blockLength The length of the block.
*/
void PeerConnection::sendRequest(uint32_t pieceIndex, uint32_t blockOffset, uint32_t blockLength)
{
    char request[13] = {0};
    request[0] = 6; //!> Length of the message
    request[1] = 0; //!> Request message type (for the piece request)

    //* Fill in the rest with piece information
    request[4] = pieceIndex;   //!> Piece index
    request[8] = blockOffset;  //!> Block offset
    request[12] = blockLength; //!> Block length

#ifdef _WIN32
    if (send(socketFd, request, sizeof(request), 0) == SOCKET_ERROR)
    {
        throw std::runtime_error("Failed to send request");
    }
#else
    if (send(socketFd, request, sizeof(request), 0) < 0)
    {
        throw std::runtime_error("Failed to send request");
    }
#endif
}

/*!
    \brief Receive data from the peer (torrent pieces).
    \return A vector of received data.
*/
std::vector<char> PeerConnection::receiveData()
{
    std::vector<char> data;
    char buffer[16384];
    ssize_t bytesReceived;

    while (true)
    {
#ifdef _WIN32
        bytesReceived = recv(socketFd, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            throw std::runtime_error("Failed to receive data");
        }
#else
        bytesReceived = recv(socketFd, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0)
        {
            throw std::runtime_error("Failed to receive data");
        }
#endif

        if (bytesReceived > 0)
        {
            data.insert(data.end(), buffer, buffer + bytesReceived);
            if (bytesReceived < static_cast<ssize_t>(sizeof(buffer)))
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    std::cout << "Received " << data.size() << " bytes of torrent data." << std::endl;
    return data;
}
