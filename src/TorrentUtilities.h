/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\torrent_library\TorrentUtilities.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\torrent_library
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 1:44:10 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef TORRENT_UTILITIES_H
#define TORRENT_UTILITIES_H

#include <string>
#include <unordered_map>
#include <vector>

class TorrentUtilities
{
public:
    /*! 
        \brief Decodes bencoded data.
        \param data The bencoded data to decode.
        \return A map of the decoded data.
    */
    static std::unordered_map<std::string, std::string> decodeBencodedData(const std::string &data);

    /*! 
        \brief Encodes data into bencoded format.
        \param data The data to encode.
        \return The bencoded data.
    */
    static std::string encodeBencodedData(const std::unordered_map<std::string, std::string> &data);
};

#endif