#!/bin/bash
# AI-Monero-Miner-XMRig Linux Build Script

mkdir -p build
cd build

echo "Configuring project..."
cmake ..

echo "Building project..."
cmake --build . --config Release

if [ $? -eq 0 ]; then
    echo "Build successful! Binary location: build/AI-Monero-Miner-XMRig"
else
    echo "Build failed!"
    # exit 1
fi
