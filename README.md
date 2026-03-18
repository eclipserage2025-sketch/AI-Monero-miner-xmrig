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

### Building
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running
```bash
./AI-Monero-Miner-XMRig
```

## Architecture
- `Miner`: Central coordinator.
- `StratumClient`: Handles pool communication.
- `AIOptimizer`: AI decision-making engine.
- `Worker`: Abstract base for hardware-specific mining logic.
- `RandomXVM`: Core mining algorithm virtual machine.
