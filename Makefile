# Set the compiler
CXX = g++

# Source files
SOURCES = \
    TorrentClientApp.cpp \
    src/DHTClient.cpp \
    src/PeerDiscovery.cpp \
    src/PeerConnection.cpp \
    src/DownloadTorrent.cpp \
    src/TorrentUtilities.cpp \
    src/MagnetParser.cpp \
    src/MagnetMetadata.cpp

# Include directories
INCLUDES = -I"C:/msys64/mingw64/include/openssl"
LIBDIRS = -L"C:/msys64/mingw64/lib"
LIBS = -lcrypto -lws2_32

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra

# Platform-specific adjustments
ifeq ($(OS),Windows_NT)
    # Windows-specific flags (for MinGW)
    LIBS += -lws2_32
    # Use backslashes for paths in Windows
    TARGET := bin/torrentclient.exe
else
    # Unix-based (Linux/macOS)
    # For Unix-based systems, no extra libraries needed for socket handling
    LIBS += -pthread
    # Set target name for Unix-based systems
    TARGET := bin/torrentclient
    # Make sure target directory exists
    ifndef $(shell test -d bin && echo true)
        $(shell mkdir bin)
    endif
endif

# Default target: build the application
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) $(LIBDIRS) $(LIBS) -o $(TARGET)

# Clean up built files
clean:
	$(RM) $(TARGET)

# Ensure clean is a non-file target
.PHONY: clean
