/*
 * File: c:\Users\tonyw\Desktop\New folder (2)\src\TorrentUtilities.cpp
 * Project: c:\Users\tonyw\Desktop\New folder (2)\src
 * Created Date: Wednesday January 29th 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 30th 2025 4:36:35 
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
    \brief Helper function to decode a bencoded dictionary.
    \param data The bencoded string.
    \param index The current parsing index.
    \return A map representing the decoded dictionary.
*/
std::unordered_map<std::string, std::string> TorrentUtilities::decodeBencodedData(const std::string &data, size_t &index)
{
    if (data[index] != 'd')
        throw std::runtime_error("Invalid dictionary format");

    std::unordered_map<std::string, std::string> decoded;
    index++; // Skip 'd'

    std::cout << "Decoding dictionary..." << std::endl;

    while (index < data.length() && data[index] != 'e')
    {
        std::string key = decodeBencodedString(data, index);
        std::cout << "Decoded Key: " << key << std::endl;

        if (index >= data.length())
            throw std::runtime_error("Unexpected end of dictionary");

        if (std::isdigit(data[index]))
        {
            decoded[key] = decodeBencodedString(data, index);
            std::cout << "Decoded Value: " << decoded[key] << std::endl;
        }
        else if (data[index] == 'd')
        {
            decoded[key] = encodeBencodedData(decodeBencodedData(data, index)); //!> Recursively decode dictionary
            std::cout << "Decoded Dictionary for key: " << key << std::endl;
        }
        else if (data[index] == 'l')
        {
            decoded[key] = encodeBencodedList(decodeBencodedList(data, index)); //!> Recursively decode list
            std::cout << "Decoded List for key: " << key << std::endl;
        }
        else
        {
            throw std::runtime_error("Unsupported bencoded format");
        }
    }

    if (index >= data.length() || data[index] != 'e')
        throw std::runtime_error("Dictionary not properly terminated");

    index++; // Skip 'e'
    return decoded;
}

/*!
    \brief Helper function to decode a bencoded list.
    \param data The bencoded string.
    \param index The current parsing index.
    \return A vector representing the decoded list.
*/
std::vector<std::string> TorrentUtilities::decodeBencodedList(const std::string &data, size_t &index)
{
    if (data[index] != 'l')
        throw std::runtime_error("Invalid list format");

    std::vector<std::string> decodedList;
    index++; //!> Skip 'l'

    while (index < data.length() && data[index] != 'e')
    {
        if (std::isdigit(data[index]))
        {
            decodedList.push_back(decodeBencodedString(data, index));
        }
        else if (data[index] == 'd')
        {
            decodedList.push_back(encodeBencodedData(decodeBencodedData(data, index))); //!> decode dictionary
        }
        else if (data[index] == 'l')
        {
            decodedList.push_back(encodeBencodedList(decodeBencodedList(data, index))); //!> decode list
        }
        else
        {
            throw std::runtime_error("Invalid bencoded list format");
        }
    }

    if (index >= data.length() || data[index] != 'e')
        throw std::runtime_error("List not properly terminated");

    index++; //!> Skip 'e'
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
    if (!std::isdigit(data[index]))
        throw std::runtime_error("Invalid string format");

    size_t colonPos = data.find(':', index);
    if (colonPos == std::string::npos)
        throw std::runtime_error("Malformed bencoded string");

    size_t strLen = std::stoul(data.substr(index, colonPos - index));
    index = colonPos + 1;

    if (index + strLen > data.length())
        throw std::runtime_error("String length exceeds data size");

    std::string value = data.substr(index, strLen);
    index += strLen;

    return value;
}

/*!
    \brief Decodes top-level bencoded data.
    \param data The bencoded string.
    \return A map of the decoded data.
*/
std::unordered_map<std::string, std::string> TorrentUtilities::decodeBencodedData(const std::string &data)
{
    size_t index = 0;
    return decodeBencodedData(data, index);
}

/*!
    \brief Encodes a dictionary into bencoded format.
    \param data The dictionary to encode.
    \return The bencoded string.
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
    \return The bencoded string.
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
