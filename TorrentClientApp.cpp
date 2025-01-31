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

        std::cout << "Info Hash: " << metadata.getInfoHash() << std::endl;
        std::cout << "Piece Size: " << metadata.getPieceSize() << std::endl;
        std::cout << "Display Name: " << metadata.getDisplayName() << std::endl;

        std::cout << "Trackers:" << std::endl;
        for (const auto &tracker : metadata.getTrackers())
        {
            std::cout << tracker << std::endl;
        }

        std::cout << "Piece Hashes:" << std::endl;
        for (const auto &pieceHash : metadata.getPieceHashes())
        {
            std::cout << pieceHash << std::endl;
        }

        std::cout << "DHT Nodes:" << std::endl;
        std::vector<std::string> nodeStrings;
        for (const auto &node : metadata.getNodes())
        {
            nodeStrings.push_back(node.ip + ":" + std::to_string(node.port));
        }

        DHTClient dhtClient(metadata.getInfoHash(), 6881, nodeStrings);
        dhtClient.discoverPeers();
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
