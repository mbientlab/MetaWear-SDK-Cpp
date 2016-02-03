# MetaWear C++ API #
This project is a C++ implementation of the MetaWear protocol.  If compiled as a shared library, it can be used with any language that supports calling C functions from a shared library, such as C# and Python.  The library only constructs the bytes for communicating with the MetaWear platform, it **does not** contain any Bluetooth LE code.  Users will need to fill in the appropriate Bluetooth LE functions for their target device.

# Build #
Building the project has been tested on Linux with GCC 4.8.5 and Clang 3.7.0, and on Windows with Visual Studio Community 2015.  

```sh
> gcc --version
gcc (SUSE Linux) 4.8.5
Copyright (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

> clang++ --version
clang version 3.7.0 (tags/RELEASE_370/final 246586)
Target: x86_64-suse-linux
Thread model: posix
```

## GCC and Clang ##
Linux users can build the project by invoking make.  The default action is to build the shared library for your platform.

```sh
> make
```

You can change the C++ compiler by overriding the CXX make variable.
```sh
> make CXX=clang++
```

Upon a successful compile, the library will be placed in the newly created "dist" directory.

```sh
> tree dist
dist/
└── release
    └── lib
        └── x64
            ├── libmetawear.so -> libmetawear.so.0
            ├── libmetawear.so.0 -> libmetawear.so.0.3.0
            └── libmetawear.so.0.3.0

```

### Testing ###
Unit tests for the library are written in Python (min v3.4.1) and can be invoked by calling the test target.

```sh
> make test
python3 -m unittest discover -s test
................................................................................
................................................................................
......
----------------------------------------------------------------------
Ran 166 tests in 0.047s

OK
``` 

## Visual Studio ##
A Visual Studio solution building the C# wrapper is available as a separate project, link [here](https://github.com/mbientlab/MetaWear-CSharpWrapper).  The C++ source code is built as Windows Runtime Component with the following changes to the project properties:

1. Disable the **Precompiled Headers** compile option  
2. Disable **Generate Windows Metadata** linker option  
3. Add **METAWEAR_DLL_EXPORTS** to the preprocessor list