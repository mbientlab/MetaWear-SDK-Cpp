# MetaWear C++ API #
This project is a C++ implementation of the MetaWear protocol.  If compiled as a shared library, it can be used with any language that supports calling 
C functions from a shared library, such as Node and Python.  The library only constructs the bytes for communicating with the MetaWear platform, it 
**does not** contain any Bluetooth LE code.  Users will need to fill in the appropriate Bluetooth LE functions for their target platform.

# Build #
Building the project has been tested on \*nix systems with Clang 4, and on Windows with Visual Studio Community 2017.  

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

## GCC and Clang ##
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
            ├── libmetawear.so.0 -> libmetawear.so.0.18.4
            └── libmetawear.so.0.18.4

```

## Visual Studio 2017 ##
MSBuild files have been provided to build the project as both a Win32 and WinRT dll.  The Win32 dll is for classic Win32 applications whereas the 
WinRT dll is for Universal Windows apps.  You will need to have [Visual Studio 2017](https://www.visualstudio.com/downloads/) installed in order to 
run the build.

When calling MSBuild, you can set the `Configuration` and `Platform` properties to control debug/release builds and x86/x64/ARM platforms 
respectively.  If not specified on the command line, the default action is to build a debug dll targeting the x86 (Win32) platform.  Note that the ARM 
platform is only available for the WinRT build.

```bat
# default properties are debug config with x86 (win32) platform
msbuild MetaWear.Win32.vcxproj

# release build for Win32 applications targeting the x64 architecture 
msbuild MetaWear.Win32.vcxproj /p:Configuration=Release;Platform=x64

# debug build for WinRT applications targeting the ARM architecture 
msbuild MetaWear.WinRT.vcxproj /p:Configuration=Debug;Platform=ARM
```

As with the Makefile, the .dll, .lib, .pdb, and .exp files are placed in the `dist` directory.

```sh
> tree dist
dist/
└── release
    └── lib
        └── x64
            ├── MetaWear.Win32.dll
            ├── MetaWear.Win32.exp
            ├── MetaWear.Win32.iobj
            ├── MetaWear.Win32.ipdb
            ├── MetaWear.Win32.lib
            └── MetaWear.Win32.pdb

```

# Testing #
Unit tests for the library are written in Python (min v3.4.1) and can be invoked by calling the test target (Test for MSBuild).

## GNU Make ##
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

## MSBuild ##
When testing with MSBuild, it is important that the `Platform` property matches the installed Python's target platform as well.  For example, if 
64-bit Python is installed, set the `Platform` property to x64 when running the `Test` target otherwise MSBuild will use the x86 dll which will cause 
all of the tests to fail.

```bat
metawear-cpp-api>msbuild MetaWear.Win32.vcxproj /p:Platform=x64 /t:Test
```
