cmake_minimum_required(VERSION 3.15)
project(AI-Monero-Miner-XMRig LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/network.cpp
    src/stratum.cpp
    src/worker.cpp
    src/miner.cpp
    src/crypto/blake2b.cpp
    src/crypto/argon2.cpp
    src/randomx/randomx_vm.cpp
    src/ai_optimizer.cpp
    src/gpu/gpu_worker.cpp
    src/config.cpp
)

# Header directories
include_directories(include)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Windows-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
    target_link_libraries(${PROJECT_NAME} ws2_32) # For networking
endif()
