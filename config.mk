APP_NAME=metawear
MODULES=metawear/core metawear/peripheral metawear/processor metawear/sensor metawear/impl

ifndef PLATFORM
    ifeq ($(shell uname -m), x86_64)
        PLATFORM:=x64
    else
        PLATFORM:=x86
    endif
endif

DOC_DIR=doc

SOURCE_DIR?=src
BUILD_DIR?=build
DIST_DIR?=dist
CONFIGURATION?=release
