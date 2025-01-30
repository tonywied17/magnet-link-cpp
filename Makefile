CXX = g++

TARGET = torrentclient.exe

SOURCES = \
    TorrentClientApp.cpp \
    src/DHTClient.cpp \
    src/PeerDiscovery.cpp \
    src/PeerConnection.cpp \
    src/DownloadTorrent.cpp \
    src/TorrentUtilities.cpp \
    src/MagnetParser.cpp \
    src/MagnetMetadata.cpp

INCLUDES = -I"C:/msys64/mingw64/include/openssl"
LIBDIRS = -L"C:/msys64/mingw64/lib"
LIBS = -lcrypto -lws2_32
CXXFLAGS = -std=c++17 -Wall -Wextra

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) $(LIBDIRS) $(LIBS) -o $(TARGET)

clean:
	del $(TARGET)

.PHONY: clean