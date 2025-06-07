# MetaWear SDK for C++ by MBIENTLAB

[![Platforms](https://img.shields.io/badge/platform-linux%20%7C%20windows%20%7C%20ios%20%7C%20osx-lightgrey)](https://github.com/mbientlab/MetaWear-SDK-Cpp)
[![License](https://img.shields.io/cocoapods/l/MetaWear.svg?style=flat)](https://github.com/mbientlab/MetaWear-SDK-Cpp/blob/master/LICENSE.md)

![alt tag](https://raw.githubusercontent.com/mbientlab/MetaWear-SDK-iOS-macOS-tvOS/master/Images/Metawear.png)

This project is a C++ implementation of the MetaWear protocol.  If compiled as a shared library, it can be used with any language that supports calling C functions from a shared library, such as Node and Python.  

The library only constructs the bytes for communicating with the MetaWear platform, it **does not** contain any Bluetooth LE code.  Users will need to fill in the appropriate Bluetooth LE functions for their target platform.

> This library is platform agnostic and does not contain any Bluetooth code. You are responsible for adding in this functionality.

Version 0.30.0 supports the following boards:
- [x] MMS
- [x] MMC
- [x] MMR + MMRL
- [ ] MetaTracker
- [ ] MetaWear HR / PPG
- [ ] MMM (mini)
- [ ] MWR, MWRG, MWRGPro
- [ ] MWC, MWCPro, MWCenv, MWCDetect 
- [ ] Custom boards

### Overview
[MetaWear](https://mbientlab.com) is a complete development and production platform for wearable and connected device applications.

MetaWear features a number of sensors and peripherals (button, acceleromter, LED, gyroscope, magnetometer, ambient light, IOs...) all easily controllable over Bluetooth 4.0/5.0 Low Energy using this SDK.

### Requirements
- [MetaWear board](https://mbientlab.com/store/)
- A linux/mac/windows machine with Bluetooth 4.0 or Bluetooth 5.0

### Notes
- Our tutorials will mostly reflect usage on iOS and Linux (see our Swift, Python and Javascript SDKs)
- Some Windows support is provided with Python.

### License
See the [License](https://github.com/mbientlab/MetaWear-SDK-Cpp/blob/master/LICENSE.md).

### Support
Reach out to the [community](https://mbientlab.com/community/) if you encounter any problems.

## Getting Started

### Installation
Building the project has been tested on Linux Debian with Clang 15.  

### Usage

```sh
> clang++ --version
clang version 15.0.6
Target: x86_64-unknown-linux-gnu
```
```bat
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community>cl.exe
Microsoft (R) C/C++ Optimizing Compiler Version 19.10.25019 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

usage: cl [ option... ] filename... [ /link linkoption... ]
```

### GCC and Clang
Linux users can build the project by invoking GNU make; the default action is to build the shared library for your current platform.  You can also change the C++ compiler by overriding the CXX make variable.

```sh
> make

# build using g++ as the compiler
> make CXX=g++
```

Upon a successful compile, the library will be placed in the newly created "dist" directory.

```sh
> tree dist
dist/
└── release
    └── lib
        └── x64
            ├── libmetawear.so -> libmetawear.so.0
            ├── libmetawear.so.0 -> libmetawear.so.0.30.0
            └── libmetawear.so.0.30.0

```

### CMake
For cross-platform inclusion of this SDK via cmake just clone thie repository and include the folder as a subdirectory to the parent project and apply the appropriate target links similar to below:
```
# Adds Metawear SDK as a dependency
message(STATUS "Device: Adding Metawear SDK Support")
add_subdirectory(MetaWear-SDK-Cpp)
target_include_directories(${PROJECT_NAME} PRIVATE MetaWear-SDK-Cpp/src)
target_link_libraries(${PROJECT_NAME} PUBLIC metawear metawear::metawear)
```

## Testing
Unit tests for the library are written in Python (min v3.4.1) and can be invoked by calling the test target.

### GNU Make
```sh
> make test
python3 -m unittest discover -s test
.................................................................................
.................................................................................
.................................................................................
.................................................................................
..............................s..................................................
........................................................
----------------------------------------------------------------------
Ran 461 tests in 33.249s

OK (skipped=1)
```

### Tutorials
Tutorials can be found [here](https://mbientlab.com/tutorials/).
