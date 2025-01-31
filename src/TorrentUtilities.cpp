/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\TorrentUtilities.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 9:09:17 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "TorrentUtilities.h"
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iostream>

/*!
    \brief Encodes a dictionary into bencoded format.
    \param data The dictionary to encode.
    \return The bencoded string
*/
std::string TorrentUtilities::encodeBencodedData(const std::unordered_map<std::string, std::string> &data)
{
    std::ostringstream encoded;
    encoded << "d";

    for (const auto &pair : data)
    {
        encoded << pair.first.length() << ":" << pair.first;
        encoded << pair.second.length() << ":" << pair.second;
    }

    encoded << "e";
    return encoded.str();
}

/*!
    \brief Encodes a list into bencoded format.
    \param data The list to encode.
    \return The bencoded string
*/
std::string TorrentUtilities::encodeBencodedList(const std::vector<std::string> &data)
{
    std::ostringstream encoded;
    encoded << "l";

    for (const auto &item : data)
    {
        encoded << item.length() << ":" << item;
    }

    encoded << "e";
    return encoded.str();
}

/*!
    \brief URL decodes a given string.
    \param url The URL-encoded string.
    \return The decoded string.
*/
std::string TorrentUtilities::urlDecode(const std::string &url)
{
    std::string decoded;
    char ch;
    unsigned int ii;
    for (size_t i = 0; i < url.length(); ++i)
    {
        if (url[i] == '%')
        {
            sscanf(url.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            decoded += ch;
            i += 2;
        }
        else if (url[i] == '+')
        {
            decoded += ' ';
        }
        else
        {
            decoded += url[i];
        }
    }
    return decoded;
}

/*!
    \brief Helper function to decode a bencoded dictionary.
    \param data The bencoded string.
    \param index The current parsing index.
    \return A map representing the decoded dictionary.
*/
std::unordered_map<std::string, std::string> TorrentUtilities::decodeBencodedData(const std::string &data, size_t &index)
{
    std::unordered_map<std::string, std::string> decodedData;

    if (data[index] != 'd')
        throw std::invalid_argument("Expected 'd' for dictionary start");
    index++;

    while (data[index] != 'e')
    {
        std::string key = decodeBencodedString(data, index);

        std::string value = decodeBencodedString(data, index);

        decodedData[key] = value;
    }

    index++;

    return decodedData;
}

/*!
    \brief Helper function to decode a bencoded list.
    \param data The bencoded string.
    \param index The current parsing index.
    \return A vector representing the decoded list.
*/
std::vector<std::string> TorrentUtilities::decodeBencodedList(const std::string &data, size_t &index)
{
    std::vector<std::string> decodedList;

    if (data[index] != 'l')
        throw std::invalid_argument("Expected 'l' for list start");
    index++;

    while (data[index] != 'e')
    {
        decodedList.push_back(decodeBencodedString(data, index));
    }

    index++;

    return decodedList;
}

/*!
    \brief Helper function to decode a bencoded string.
    \param data The bencoded string.
    \param index The current parsing index.
    \return The decoded string.
*/
std::string TorrentUtilities::decodeBencodedString(const std::string &data, size_t &index)
{
    size_t lenEnd = data.find(':', index);
    if (lenEnd == std::string::npos)
        throw std::invalid_argument("Invalid bencoded string length format");

    int length = std::stoi(data.substr(index, lenEnd - index));
    index = lenEnd + 1;

    std::string decodedString = data.substr(index, length);
    index += length;

    return decodedString;
}

/*!
    \brief Converts a dictionary to a string.
    \param data The dictionary to convert.
    \return The string representation of the dictionary.
*/
std::string TorrentUtilities::toString(const std::unordered_map<std::string, std::string> &data)
{
    std::ostringstream oss;
    oss << "{";
    for (const auto &pair : data)
    {
        oss << pair.first << ": " << pair.second << ", ";
    }
    oss << "}";
    return oss.str();
}