APP_NAME?=metawear
MODULES?=metawear/platform metawear/core metawear/peripheral metawear/processor metawear/sensor metawear/impl metawear/dfu

KERNEL?=$(shell uname -s)

ifndef MACHINE
    MACHINE_RAW:=$(shell uname -m)
    ifeq ($(MACHINE_RAW),x86_64)
        MACHINE:=x64
    else ifeq ($(MACHINE_RAW),amd64)
        MACHINE:=x64
    else ifneq (,$(findstring arm, $(MACHINE_RAW)))
        MACHINE:=arm
    else ifeq ($(MACHINE_RAW),aarch64)
        MACHINE:=arm
    else
        MACHINE:=x86
    endif
endif


DOC_DIR=doc

BINDINGS_DIR?=bindings
SOURCE_DIR?=src
BUILD_DIR?=build
DIST_DIR?=dist
CONFIGURATION?=release
CXXFLAGS?=-std=c++11 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -Wall -Werror -I$(SOURCE_DIR) -DMETAWEAR_DLL -DMETAWEAR_DLL_EXPORTS $(OPT_FLAGS)
DBG?=gdb
