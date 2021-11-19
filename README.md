# MetaWear SDK for C++ by MBIENTLAB

[![Platforms](https://img.shields.io/badge/platform-linux%20%7C%20windows%20%7C%20ios%20%7C%20osx-lightgrey)](https://github.com/mbientlab/MetaWear-SDK-Cpp)
[![License](https://img.shields.io/cocoapods/l/MetaWear.svg?style=flat)](https://github.com/mbientlab/MetaWear-SDK-Cpp/blob/master/LICENSE.md)

![alt tag](https://raw.githubusercontent.com/mbientlab/MetaWear-SDK-iOS-macOS-tvOS/master/Images/Metawear.png)

This project is a C++ implementation of the MetaWear protocol.  If compiled as a shared library, it can be used with any language that supports calling C functions from a shared library, such as Node and Python.  

The library only constructs the bytes for communicating with the MetaWear platform, it **does not** contain any Bluetooth LE code.  Users will need to fill in the appropriate Bluetooth LE functions for their target platform.

> ADDITIONAL NOTES  
This library is platform agnostic and does not contain any Bluetooth code. You are responsible for adding in this functionality.

### Overview

[MetaWear](https://mbientlab.com) is a complete development and production platform for wearable and connected device applications.

MetaWear features a number of sensors and peripherals all easily controllable over Bluetooth 4.0/5.0 Low Energy using this SDK, no firmware or hardware experience needed!

The MetaWear hardware comes pre-loaded with a wirelessly upgradeable firmware, so it keeps getting more powerful over time.

### Requirements
- [MetaWear board](https://mbientlab.com/store/)
- A linux/mac/windows machine with Bluetooth 4.0 or Bluetooth 5.0

### Notes
- Our tutorials will mostly reflect usage on ios and linux (see our SDKs)
- Can work on macOS/iOS/OSX/tvOS (you may need to do some additional dev yourself - not documented/supported by Mbient)
- Can work on Windows (you will need to do some additional dev yourself - not documented/supported by Mbient)

### License
See the [License](https://github.com/mbientlab/MetaWear-SDK-Cpp/blob/master/LICENSE.md).

### Support
Reach out to the [community](https://mbientlab.com/community/) if you encounter any problems, or just want to chat :)

## Getting Started

### Installation
Building the project has been tested on Unix systems with Clang 4.  

### Usage

```sh
> clang++ --version
clang version 4.0.1 (tags/RELEASE_401/final 305264)
Target: x86_64-unknown-linux-gnu
```
```bat
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community>cl.exe
Microsoft (R) C/C++ Optimizing Compiler Version 19.10.25019 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

usage: cl [ option... ] filename... [ /link linkoption... ]
```

### GCC and Clang
Linux users can build the project by invoking GNU make; the default action is to build the shared library for your current platform.  You can also 
change the C++ compiler by overriding the CXX make variable.

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
            ├── libmetawear.so.0 -> libmetawear.so.0.20.6
            └── libmetawear.so.0.20.6

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
