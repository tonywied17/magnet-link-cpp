#include "MagnetParser.h"
#include <sstream>
#include <stdexcept>
#include <cstdint>

/*!
    \brief Creates a MagnetParser object with the given magnet link.
    \param magnetLink The magnet link to parse.
*/
MagnetParser::MagnetParser(const std::string &magnetLink) : magnetLink(magnetLink) {}

/*!
    \brief Parses the magnet link and returns the metadata.
    \return The metadata of the torrent.
*/
MagnetMetadata MagnetParser::parse()
{
    std::string infoHash;
    std::vector<std::string> trackers;
    std::vector<std::string> pieceHashes;

    if (magnetLink.find("magnet:?") != 0)
    {
        throw std::runtime_error("Invalid magnet link format");
    }

    size_t start = magnetLink.find("magnet:?") + 8;
    std::istringstream stream(magnetLink.substr(start));
    std::string param;

    while (std::getline(stream, param, '&'))
    {
        size_t equalPos = param.find('=');
        if (equalPos == std::string::npos)
            continue;

        std::string key = param.substr(0, equalPos);
        std::string value = param.substr(equalPos + 1);

        if (key == "xt" && value.find("urn:btih:") == 0)
        {
            infoHash = value.substr(9); //!> Extract hash after "urn:btih:"
        }
        else if (key == "tr")
        {
            trackers.push_back(value); //!> Trackers
        }
        else if (key == "p")
        {
            pieceHashes.push_back(value); //!> Extract piece hashes
        }
        }

    if (infoHash.empty())
    {
        throw std::runtime_error("Missing or invalid info hash in magnet link");
    }

    return MagnetMetadata(infoHash, trackers, pieceHashes);
}
