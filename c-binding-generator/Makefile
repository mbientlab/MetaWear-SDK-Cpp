.PHONY: build clean

include config.mk

CXXFLAGS+=-std=c++11 -Wall -Werror -I$(SOURCE_DIR) -I/usr/lib/llvm-7/include

ifeq ($(CONFIGURATION),debug)
    CXXFLAGS+=-g
    LDFLAGS:=-g
else ifeq ($(CONFIGURATION),release)
    CXXFLAGS+=-O3
    LDFLAGS:=
else
    $(error Invalid value for "CONFIGURATION", must be 'release' or 'debug')
endif
LDFLAGS+=-lclang -ldl -rdynamic -L/usr/lib/llvm-7/lib

ifeq ($(MACHINE),x86)
    CXXFLAGS+=-m32
else ifeq ($(MACHINE),x64)
    CXXFLAGS+=-m64
else ifeq ($(MACHINE),arm)
    CXXFLAGS+=-marm
else
    $(error Unrecognized "MACHINE" value, use 'x86', 'x64', or 'arm')
endif

REAL_DIST_DIR:=$(DIST_DIR)/$(CONFIGURATION)/bin/$(MACHINE)
REAL_BUILD_DIR:=$(BUILD_DIR)/$(MACHINE)/$(CONFIGURATION)
MODULES_BUILD_DIR:=$(addprefix $(REAL_BUILD_DIR)/, $(SOURCE_DIR)/cbinds/cpp)
APP_OUTPUT:=$(REAL_DIST_DIR)/$(APP_NAME)

SRCS:=$(shell find $(SOURCE_DIR)/cbinds/cpp -name \*.cpp)
OBJS:=$(addprefix $(REAL_BUILD_DIR)/,$(SRCS:%.cpp=%.o))
DEPS:=$(OBJS:%.o=%.d)

build: $(MODULES_BUILD_DIR) $(REAL_DIST_DIR) $(APP_OUTPUT)

-include $(DEPS)

$(REAL_BUILD_DIR)/%.o: %.cpp
	clang++ -MMD -MP -MF "$(@:%.o=%.d)" -c -o $@ $(CXXFLAGS) $<

$(APP_OUTPUT): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(MODULES_BUILD_DIR):
	mkdir -p $@

$(REAL_DIST_DIR):
	mkdir -p $@

clean:
	rm -Rf $(MODULES_BUILD_DIR) $(APP_OUTPUT)
