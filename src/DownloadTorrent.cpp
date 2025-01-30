#include "DownloadTorrent.h"
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <openssl/sha.h>
#include "PeerDiscovery.h"

/*!
    \brief Creates a DownloadTorrent object with the given metadata.
    \param metadata The metadata of the torrent.
*/
DownloadTorrent::DownloadTorrent(const MagnetMetadata &metadata)
    : metadata(metadata), downloadDirectory("downloads") {}

/*!
    \brief Starts the download process.
*/
void DownloadTorrent::startDownload()
{
    createDownloadDirectory();

    PeerDiscovery peerDiscovery(metadata.getInfoHash());
    peers = peerDiscovery.discoverPeers();
    if (peers.empty())
    {
        std::cerr << "No peers found!" << std::endl;
        return;
    }

    requestPieces();
}

/*!
    \brief Ensure the download directory exists.
*/
void DownloadTorrent::createDownloadDirectory()
{
    if (!std::filesystem::exists(downloadDirectory))
    {
        std::filesystem::create_directory(downloadDirectory);
    }
}

/*!
    \brief Request pieces from peers.
*/
void DownloadTorrent::requestPieces()
{
    for (uint32_t pieceIndex = 0; pieceIndex < metadata.getPieceHashes().size(); ++pieceIndex)
    {
        for (const auto &peer : peers)
        {
            PeerConnection peerConnection(peer, metadata);
            if (peerConnection.connectToPeer())
            {
                peerConnection.performHandshake();

                peerConnection.sendRequest(pieceIndex, 0, 16384);
                std::vector<char> pieceData = peerConnection.receiveData();

                if (verifyPiece(pieceIndex, pieceData))
                {
                    savePiece(pieceIndex, pieceData);
                    updatePieceStatus(pieceIndex, true);
                    break;
                }
                else
                {
                    retryPieceDownload(pieceIndex);
                }
            }
        }
    }
}

/*!
    \brief Save the downloaded piece.
    \param pieceIndex The index of the piece.
    \param pieceData The data of the piece.
*/
void DownloadTorrent::savePiece(uint32_t pieceIndex, const std::vector<char> &pieceData)
{
    std::string filename = downloadDirectory + "/piece" + std::to_string(pieceIndex) + ".dat";
    std::ofstream
        file(filename, std::ios::out | std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }
    file.write(pieceData.data(), pieceData.size());

    std::cout << "Piece " << pieceIndex << " downloaded and saved." << std::endl;

    file.close();
}

/*!
    \brief Track the status of pieces.
    \param pieceIndex The index of the piece.
    \param isDownloaded True if the piece was downloaded successfully.
*/
void DownloadTorrent::updatePieceStatus(uint32_t pieceIndex, bool isDownloaded)
{
    std::cout << "Piece " << pieceIndex << " status updated: " << (isDownloaded ? "downloaded" : "failed") << std::endl;
}

/*!
    \brief Verify piece integrity.
    \param pieceIndex The index of the piece.
    \param pieceData The data of the piece.
    \return True if the piece hash matches the expected hash.
*/
bool DownloadTorrent::verifyPiece(uint32_t pieceIndex, const std::vector<char> &pieceData)
{
    std::string pieceHash = calculateSHA1(pieceData);
    if (pieceHash != metadata.getPieceHashes()[pieceIndex])
    {
        std::cerr << "Piece " << pieceIndex << " hash mismatch!" << std::endl;
        return false;
    }
    return true;
}

/*!
    \brief Retry downloading a piece if it fails.
    \param pieceIndex The index of the piece.
*/
void DownloadTorrent::retryPieceDownload(uint32_t pieceIndex)
{
    std::cout << "Retrying download of piece " << pieceIndex << "..." << std::endl;

    for (int retries = 0; retries < 3; ++retries)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        for (const auto &peer : peers)
        {
            PeerConnection peerConnection(peer, metadata);
            if (peerConnection.connectToPeer())
            {
                peerConnection.performHandshake();

                peerConnection.sendRequest(pieceIndex, 0, 16384);
                std::vector<char> pieceData = peerConnection.receiveData();

                if (verifyPiece(pieceIndex, pieceData))
                {
                    savePiece(pieceIndex, pieceData);
                    updatePieceStatus(pieceIndex, true);
                    return;
                }
            }
        }
    }

    std::cerr << "Failed to download piece " << pieceIndex << " after 3 retries!" << std::endl;
}

/*!
    \brief Calculate the SHA-1 hash of the piece data.
    \param data The data of the piece.
    \return The SHA-1 hash.
*/
std::string DownloadTorrent::calculateSHA1(const std::vector<char> &data)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(data.data()), data.size(), hash);

    char hexString[SHA_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        sprintf(hexString + i * 2, "%02x", hash[i]);
    }
    return std::string(hexString);
}
