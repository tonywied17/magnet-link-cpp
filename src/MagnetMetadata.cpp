#include "MagnetMetadata.h"
#include <cstdint>

/*!
    \brief Creates a MagnetMetadata object with the given hash, trackers, and piece hashes.
    \param hash The SHA-1 hash of the torrent.
    \param trackers The list of tracker URLs.
    \param pieceHashes The list of piece hashes (one per piece).
*/
MagnetMetadata::MagnetMetadata(std::string hash, std::vector<std::string> trackers, std::vector<std::string> pieceHashes)
    : infoHash(std::move(hash)), trackers(std::move(trackers)), pieceHashes(std::move(pieceHashes)) {}

/*!
    \brief Get the info hash
*/
const std::vector<std::string> &MagnetMetadata::getPieceHashes() const
{
    return pieceHashes;
}

/*!
    \brief Get the info hash
*/
const std::string &MagnetMetadata::getInfoHash() const
{
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
    \brief Get the size of each piece
*/
uint32_t MagnetMetadata::getPieceSize() const
{
    return pieceSize;
}
