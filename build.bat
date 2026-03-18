@echo off
:: AI-Monero-Miner-XMRig Windows Build Script

if not exist build mkdir build
cd build

echo "Configuring project..."
cmake ..

echo "Building project..."
cmake --build . --config Release

if %ERRORLEVEL% equ 0 (
    echo Build successful! Binary location: build\Release\AI-Monero-Miner-XMRig.exe
) else (
    echo Build failed!
)
