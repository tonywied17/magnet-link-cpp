/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\TorrentClientApp.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 1:59:04 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "src/MagnetParser.h"
#include "src/DHTClient.h"
#include "src/PeerDiscovery.h"
#include "src/PeerConnection.h"
#include "src/DownloadTorrent.h"
#include "src/TorrentUtilities.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/*!
    \brief URL decodes a given string.
    \param url The URL-encoded string.
    \return The decoded string.
*/
std::string urlDecode(const std::string &url)
{
    std::string decoded;
    char ch;
    std::string::size_type i;
    unsigned int ii; 
    for (i = 0; i < url.length(); i++) {
        if (url[i] == '%') {
            sscanf(url.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            decoded += ch;
            i = i + 2;
        }
        else if (url[i] == '+') {
            decoded += ' ';
        }
        else {
            decoded += url[i];
        }
    }
    return decoded;
}

/*!
    \brief Clears the console screen.
*/
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*!
    \brief Main function for the Torrent Client application.
*/
int main()
{
    std::string magnetLink;

    //@ Step 1: Prompt for Magnet link
    clearScreen();
    std::cout << "Enter a Magnet Link: ";
    std::getline(std::cin, magnetLink);

    // Decode the magnet link
    magnetLink = urlDecode(magnetLink);

    try
    {
        //@ Step 2: Parse Magnet Link
        MagnetParser parser(magnetLink);
        MagnetMetadata metadata = parser.parse();

        std::cout << "Parsed Magnet Info Hash: " << metadata.getInfoHash() << std::endl;

        std::cout << "Trackers:" << std::endl;

        for (const auto &tracker : metadata.getTrackers())
        {
            std::cout << tracker << std::endl;
        }

        //@ Step 3: Discover Peers using DHT
        DHTClient dhtClient(metadata.getInfoHash());
        std::vector<std::string> peers = dhtClient.getPeers();

        std::cout << "Found " << peers.size() << " peers:" << std::endl;
        for (const auto &peer : peers)
        {
            std::cout << peer << std::endl;
        }

        //@ Step 4: Connect to Peers (PeerDiscovery and PeerConnection)
        PeerDiscovery peerDiscovery(metadata.getInfoHash());
        std::vector<std::string> discoveredPeers = peerDiscovery.discoverPeers();

        std::cout << "Discovered " << discoveredPeers.size() << " peers for connection." << std::endl;

        for (const auto &peer : discoveredPeers)
        {
            PeerConnection connection(peer, metadata);
            if (connection.connectToPeer())
            {
                std::cout << "Connected to peer: " << peer << std::endl;
            }
            else
            {
                std::cerr << "Failed to connect to peer: " << peer << std::endl;
            }
        }

        //@ Step 5: Download Torrent
        DownloadTorrent download(metadata);
        download.startDownload();

        std::cout << "Download started!" << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
