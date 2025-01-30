/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\TorrentUtilities.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 4:36:43 
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
        \brief Decodes top-level bencoded data.
        \param data The bencoded data to decode.
        \return A map of the decoded data.
    */
    static std::unordered_map<std::string, std::string> decodeBencodedData(const std::string &data);

    /*!
        \brief Encodes a dictionary into bencoded format.
        \param data The dictionary to encode.
        \return The bencoded string.
    */
    static std::string encodeBencodedData(const std::unordered_map<std::string, std::string> &data);

    /*!
        \brief Encodes a list into bencoded format.
        \param data The list to encode.
        \return The bencoded string.
    */
    static std::string encodeBencodedList(const std::vector<std::string> &data);

    /*!
        \brief Helper function to decode a bencoded dictionary.
        \param data The bencoded string.
        \param index The current parsing index.
        \return A map representing the decoded dictionary.
    */
    static std::unordered_map<std::string, std::string> decodeBencodedData(const std::string &data, size_t &index);

    /*!
        \brief Helper function to decode a bencoded list.
        \param data The bencoded string.
        \param index The current parsing index.
        \return A vector representing the decoded list.
    */
    static std::vector<std::string> decodeBencodedList(const std::string &data, size_t &index);

    /*!
        \brief Helper function to decode a bencoded string.
        \param data The bencoded string.
        \param index The current parsing index.
        \return The decoded string.
    */
    static std::string decodeBencodedString(const std::string &data, size_t &index);
};

#endif