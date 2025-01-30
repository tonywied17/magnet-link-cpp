/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\torrent_library\MagnetParser.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\torrent_library
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 1:43:54 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef MAGNET_PARSER_H
#define MAGNET_PARSER_H

#include <string>
#include <cstdint>
#include "MagnetMetadata.h"

class MagnetParser
{
public:
    /*!
        \brief Creates a MagnetParser object with the given magnet link.
        \param magnetLink The magnet link to parse.
    */
    explicit MagnetParser(const std::string &magnetLink);

    /*!
        \brief Parses the magnet link and returns the metadata.
        \return The metadata of the torrent.
    */
    MagnetMetadata parse();

private:
    std::string magnetLink; //!> The magnet link to parse.
};

#endif
