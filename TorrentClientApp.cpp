/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\TorrentClientApp.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 9:03:20 
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
#include "src/TorrentUtilities.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/*!
 *  \brief Clears the console screen.
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
 *  \brief Displays the magnet link details and performs torrent download.
 *  \param magnetLink The inputted magnet link.
 */
void processMagnetLink(const std::string &magnetLink)
{
    try
    {
        MagnetParser parser(magnetLink);
        MagnetMetadata metadata = parser.parse();

        std::cout << "Display Name: " << metadata.getDisplayName() << std::endl;

        std::cout << "Info Hash: " << metadata.getInfoHash() << std::endl;
        std::cout << "Trackers: " << std::endl;
        for (const auto &tracker : metadata.getTrackers())
        {
            std::cout << "  " << tracker << std::endl;
        }

        DHTClient dhtClient(metadata.getInfoHash(), 6881);
        dhtClient.discoverPeers();

        std::cout << "Piece Hashes: " << std::endl;

        for (const auto &pieceHash : metadata.getPieceHashes())
        {
            std::cout << "  " << pieceHash << std::endl;
        }

        std::cout << "Piece Size: " << metadata.getPieceSize() << " bytes" << std::endl;
    }
    catch (const std::invalid_argument &ex)
    {
        std::cerr << "Invalid Argument Error: " << ex.what() << std::endl;
    }
    catch (const std::runtime_error &ex)
    {
        std::cerr << "Runtime Error: " << ex.what() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "General Error: " << ex.what() << std::endl;
    }
}

/*!
 *  \brief Main function for the Torrent Client application.
 */
int main()
{
    std::string magnetLink;
    clearScreen();
    std::cout << "Enter a Magnet Link: ";
    std::getline(std::cin, magnetLink);

    magnetLink = TorrentUtilities::urlDecode(magnetLink);

    processMagnetLink(magnetLink);

    return 0;
}
