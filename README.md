
# Magnet Link Torrent Client

This is a basic torrent client written in C++ that parses magnet links, discovers peers via DHT, connects to them, and starts downloading torrents. It is designed as a learning project and provides a starting point for a custom torrent client.

## Steps to Run

1. **Clone the repository:**
   ```bash
   git clone https://github.com/tonywied17/magnet-link-cpp.git
   cd magnet-link-cpp
   ```

2. **Build the project using `make`:**
   ```bash
   make
   ```

3. **Run the executable:**
   ```bash
   ./torrentclient.exe
   ```

4. **Enter a Magnet link when prompted.**

## Project Structure

- `TorrentClientApp.cpp`: Main application logic.
- `src/`: Contains the library for parsing magnet links, DHT client, peer discovery, and torrent downloading.

## Features

- Parse Magnet Links
- Discover Peers via DHT
- Connect to peers and start downloading


<!-- https://www.bittorrent.org/beps/bep_0005.html

https://www.bittorrent.org/beps/bep_0003.html

https://www.bittorrent.org/beps/bep_0010.html

https://www.bittorrent.org/beps/bep_0009.html -->

## References

- [BEP 5: DHT Protocol](https://www.bittorrent.org/beps/bep_0005.html)
- [BEP 3: The BitTorrent Protocol Specification](https://www.bittorrent.org/beps/bep_0003.html)
- [BEP 10: Extension Protocol](https://www.bittorrent.org/beps/bep_0010.html)
- [BEP 9: Extension for Peers to Send Metadata Files](https://www.bittorrent.org/beps/bep_0009.html)