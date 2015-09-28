# MetaWear C++ API #
This project is a C++ implementation of the MetaWear protocol.  It compiled as a shared library, it can be used with any language that supports calling C functions from a shared library, such as C# and Python.  The library only constructs the bytes for communicating with the MetaWear platform, it **does not** contain any Bluetooth LE code.  Users will need to fill in the appropriate Bluetooth LE functions for their target device.

# Build #
Building the project has been tested on Linux using GCC and make, and on Windows with Visual Studio Community 2015.  

## GCC and Make ##
Linux users can build the project by invoking make.  The default action is to build the shared library for your platform.

```sh
> make
```

Upon a successful compile, the library will be placed in the newly created dist directory.

```sh
> tree dist
dist/
└── release
    └── lib
        └── x64
            ├── libmetawear.so -> libmetawear.so.0
            ├── libmetawear.so.0 -> libmetawear.so.0.0.0
            └── libmetawear.so.0.0.0

```

### Testing ###
Unit tests for the library are written in Python3 and can be invoked by calling the test target.

```sh
> make test
python3 -m unittest discover -s test
...................................................................................
----------------------------------------------------------------------
Ran 83 tests in 0.018s

OK
``` 

## Visual Studio ##
If building the project with Visual Studio, users should build the project as a Windows Runtime Component.  When your project is created, make the following changes to the project properties:

1. Disable the **Precompiled Headers** compile option  
2. Disable **Generate Windows Metadata** linker option  
3. Add **METAWEAR_DLL_EXPORTS** to the preprocessor list