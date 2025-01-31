
# Torrent Client

This is a basic torrent client written in C++ that parses magnet links, discovers peers via DHT, connects to them, and starts downloading torrents. It is designed as a learning project and provides a starting point for a custom torrent client.

## Steps to Run

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd <repository-directory>
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
- `lib/`: Contains the library for parsing magnet links, DHT client, peer discovery, and torrent downloading.

## Features/Goal

- Parse Magnet Links
- Discover Peers via DHT
- Connect to peers and start downloading
