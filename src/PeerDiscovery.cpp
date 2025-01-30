#include "PeerDiscovery.h"
#include <stdexcept>

/*!
    \brief Creates a PeerDiscovery object with the given info hash.
    \param infoHash The info hash of the torrent.
*/
PeerDiscovery::PeerDiscovery(const std::string &infoHash) : infoHash(infoHash), dhtClient(infoHash) {}

/*!
    \brief Discovers peers for the torrent.
    \return A vector of peer addresses.
*/
std::vector<std::string> PeerDiscovery::discoverPeers()
{
    try
    {
        return dhtClient.getPeers();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Failed to discover peers: " + std::string(e.what()));
    }
}
