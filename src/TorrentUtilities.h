/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\TorrentUtilities.h
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 9:09:22 
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
        \brief URL decodes a given string.
        \param url The URL-encoded string.
        \return The decoded string.
    */
    static std::string urlDecode(const std::string &url);

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

    /*!
        \brief Converts a dictionary to a string.
        \param data The dictionary to convert.
        \return The string representation of the dictionary.
    */
    static std::string toString(const std::unordered_map<std::string, std::string> &data);
};

#endif
