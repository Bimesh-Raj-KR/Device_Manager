# Device_Manager

A cross-platform C project that builds a Json file to list all device details, using either **CMake** or **Make** as the build system.

## Features

Organized source code in multiple directories
Separate **device** and **debug** builds
Clean, portable build system

## Prerequisites

**CMake** ≥ 3.10
**Make**
**GCC** (for Device build)


## Building with Make

### 1. Device

$ make device_list

Output: device/deviceList.exe
        device/main.o
        device/deviceManager.o
        device/fileOperations.o
        device/cJSON.o
        device/main.s
        device/deviceManager.s
        device/fileOperations.s
        device/cJSON.s
        debug/main.o
        debug/deviceManager.o
        debug/fileOperations.o
        debug/cJSON.o

### 2. Clean Build Artifacts

$ make clean

Removes all files from device/, debug/ and build/ folders.

## Building with CMake

### 1. Release Build

$ cmake -B build
$ cmake --build build