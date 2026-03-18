# AI-Monero-Miner-XMRig

A high-performance Monero (RandomX) miner built from the ground up with integrated AI for parameter optimization.

## Features
- **Built from Scratch**: Core mining logic, Stratum protocol, and hardware workers implemented from zero.
- **AI Optimization**: Integrated custom Neural Network to dynamically adjust mining parameters (thread count, memory) based on hashrate, CPU temperature, and system load.
- **Multi-Hardware Support**: Modular architecture supporting both CPU and GPU (OpenCL/CUDA) workers.
- **RandomX Implementation**: Custom implementation of RandomX cryptographic primitives (Blake2b, Argon2, VM).
- **Cross-Platform**: Designed for Windows (WinSock2) and Linux (BSD Sockets) compatibility.

## Getting Started

### Prerequisites
- C++17 or higher
- CMake 3.15+
- (Windows) MSVC or MinGW
- (Linux) GCC or Clang

### Building from Source

#### Automated Build (Recommended)
Use the provided scripts to automate the build process:
- **Windows**: Run `build.bat` from the root directory.
- **Linux/macOS**: Run `./build.sh` from the root directory.

#### Manual Build
If you prefer to build manually with CMake:
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

The resulting binary will be located in the `build/` directory (or `build/Release/` on Windows).

## Configuration
The miner uses a `config.json` file for settings. You can modify this file to point to your pool and wallet:

```json
{
    "pool": {
        "url": "pool.supportxmr.com",
        "port": 3333,
        "user": "YOUR_WALLET_ADDRESS",
        "pass": "x"
    },
    "ai": {
        "enabled": true,
        "optimization_interval": 60,
        "max_threads": 32,
        "min_threads": 1
    },
    "cpu": {
        "enabled": true,
        "threads": 4
    },
    "gpu": {
        "enabled": false,
        "platform": "OpenCL"
    }
}
```

## Running the Miner
Once built and configured, run the binary:
```bash
./build/AI-Monero-Miner-XMRig
```

## Architecture
- `Miner`: Central coordinator that manages workers and AI optimization.
- `StratumClient`: Custom implementation of the Stratum protocol for pool communication.
- `AIOptimizer`: Integrated AI decision-making engine using a custom Neural Network.
- `Worker`: Abstract base class for hardware-specific mining logic (CPU/GPU).
- `RandomXVM`: Custom implementation of the RandomX Virtual Machine and cryptographic primitives.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
