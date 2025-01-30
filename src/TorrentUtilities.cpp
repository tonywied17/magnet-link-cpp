#include "TorrentUtilities.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

/*!
    \brief Decodes bencoded data.
    \param data The bencoded data to decode.
    \return A map of the decoded data.
*/
std::unordered_map<std::string, std::string> TorrentUtilities::decodeBencodedData(const std::string &data)
{
    std::unordered_map<std::string, std::string> decodedData;
    size_t index = 0;

    while (index < data.length())
    {
        if (std::isdigit(data[index]))
        {
            size_t colonPos = data.find(':', index);
            if (colonPos == std::string::npos)
                throw std::runtime_error("Invalid bencoded string");

            size_t strLen = std::stoul(data.substr(index, colonPos - index));
            index = colonPos + 1;

            if (index + strLen > data.length())
                throw std::runtime_error("Invalid string length");

            std::string key = data.substr(index, strLen);
            index += strLen;

            //! Expect another string as value
            if (!std::isdigit(data[index]))
                throw std::runtime_error("Invalid key-value format");

            colonPos = data.find(':', index);
            if (colonPos == std::string::npos)
                throw std::runtime_error("Invalid format");

            strLen = std::stoul(data.substr(index, colonPos - index));
            index = colonPos + 1;

            if (index + strLen > data.length())
                throw std::runtime_error("Invalid value length");

            std::string value = data.substr(index, strLen);
            index += strLen;

            decodedData[key] = value;
        }
        else
        {
            throw std::runtime_error("Unsupported bencoded format");
        }
    }

    return decodedData;
}

/*!
    \brief Encodes data into bencoded format.
    \param data The data to encode.
    \return The bencoded data.
*/
std::string TorrentUtilities::encodeBencodedData(const std::unordered_map<std::string, std::string> &data)
{
    std::ostringstream encoded;

    for (const auto &pair : data)
    {
        encoded << pair.first.length() << ":" << pair.first;
        encoded << pair.second.length() << ":" << pair.second;
    }

    return encoded.str();
}
