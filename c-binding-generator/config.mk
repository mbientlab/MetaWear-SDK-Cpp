APP_NAME=cbinds

KERNEL?=$(shell uname -s)

ifndef MACHINE
    MACHINE_RAW:=$(shell uname -m)
    ifeq ($(MACHINE_RAW),x86_64)
        MACHINE:=x64
    else ifeq ($(MACHINE_RAW),amd64)
        MACHINE:=x64
    else ifneq (,$(findstring arm, $(MACHINE_RAW)))
        MACHINE:=arm
    else
        MACHINE:=x86
    endif
endif

SOURCE_DIR?=src
BUILD_DIR?=build
DIST_DIR?=dist
CONFIGURATION?=release
