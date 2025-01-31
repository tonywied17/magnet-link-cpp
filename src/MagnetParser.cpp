/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\MagnetParser.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 8:15:12 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "MagnetParser.h"
#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

MagnetParser::MagnetParser(const std::string &magnetLink) : magnetLink(magnetLink) {}

MagnetMetadata MagnetParser::parse()
{
    std::string infoHash;
    std::vector<std::string> trackers;
    std::vector<std::string> pieceHashes;
    uint32_t pieceSize = 0;
    std::string displayName;
    std::vector<DHTNode> nodes;
    uint32_t port = 6881;

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
        else if (key == "sz")
        {
            pieceSize = std::stoi(value); //!> Extract piece size
        }
        else if (key == "dn")
        {
            displayName = value; //!> Display name (torrent name)
        }
        else if (key == "node")
        {
            size_t colonPos = value.find(':');
            if (colonPos != std::string::npos)
            {
                std::string ip = value.substr(0, colonPos);
                uint16_t nodePort = std::stoi(value.substr(colonPos + 1));
                nodes.emplace_back(ip, nodePort); 
            }
        }

        else if (key == "port")
        {
            port = std::stoi(value); //!> Override port if specified
        }
    }

    if (infoHash.empty())
    {
        throw std::runtime_error("Missing or invalid info hash in magnet link");
    }

    return MagnetMetadata(infoHash, trackers, pieceHashes, pieceSize, displayName, nodes, port);
}
