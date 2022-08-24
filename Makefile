.PHONY: build clean test doc archive publish install generator bindings pythonbindings javascriptbindings swiftbindings

include config.mk
include project_version.mk

EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)
COMMA:=,

SCRIPTS_DIR:=scripts
PYTHON_BINDINGS:=$(BUILD_DIR)/bindings/python/mbientlab/metawear/cbindings.py
JAVASCRIPT_BINDINGS:=$(BUILD_DIR)/bindings/javascript/cbindings.js
SWIFT_BINDINGS:=$(BUILD_DIR)/bindings/swift/cbindings.swift

MODULES_SRC_DIR= $(addsuffix /cpp, $(addprefix $(SOURCE_DIR)/, $(MODULES)))
SRCS:=$(foreach src_dir, $(MODULES_SRC_DIR), $(shell find $(src_dir) -name \*.cpp))
EXPORT_HEADERS:=$(foreach module, $(addprefix $(SOURCE_DIR)/, $(MODULES)), $(shell find $(module) -maxdepth 1 -name \*.h))
MASTER_HEADERS:=$(foreach header, $(EXPORT_HEADERS), '\#include "$(header)"\n')

ifeq ($(CONFIGURATION),debug)
    APP_NAME:=$(APP_NAME)_d
    CXXFLAGS+=-g
    LD_FLAGS:=-g
else ifeq ($(CONFIGURATION),release)
    CXXFLAGS+=-O3
    LD_FLAGS:=-s
else
    $(error Invalid value for "CONFIGURATION", must be 'release' or 'debug')
endif

LD_FLAGS+=-shared -Wl,
ifneq ($(KERNEL),Darwin)
    EXTENSION:=so
    LIB_SO_NAME:=lib$(APP_NAME).so
    LD_FLAGS:=$(LD_FLAGS)--soname
else
    EXTENSION:=dylib
    LD_FLAGS:=-flat_namespace -undefined suppress -dynamiclib $(LD_FLAGS)-install_name
endif
LIB_SO_NAME:=lib$(APP_NAME).$(EXTENSION)
# LIB_SHORT_NAME:=$(LIB_SO_NAME).$(VERSION_MAJOR)
# LIB_NAME:=$(LIB_SO_NAME).$(VERSION)

ifeq ($(MACHINE),x86)
	ARCH=-m32
else ifeq ($(MACHINE),x64)
	ARCH=-m64
else ifeq ($(MACHINE),aarch64)
	ARCH=-march=native
else ifeq ($(MACHINE),arm)
	ARCH=-marm
else
    $(error Unrecognized "MACHINE" value, use 'x86', 'x64', 'aarch64', or 'arm')
endif

ifndef NO_MULTILIB
	CXXFLAGS+=$(ARCH)
else
    ARCH=
endif

LD_FLAGS:=$(LD_FLAGS),$(LIB_SHORT_NAME) $(ARCH)

REAL_DIST_DIR:=$(DIST_DIR)/$(CONFIGURATION)/lib/$(MACHINE)
REAL_BUILD_DIR:=$(BUILD_DIR)/$(MACHINE)/$(CONFIGURATION)
MODULES_BUILD_DIR:=$(addprefix $(REAL_BUILD_DIR)/, $(MODULES_SRC_DIR))
LIBMETAWEAR_JAVASCRIPT_PATH:=$(BINDINGS_DIR)/javascript/libmetawear-path.js

OBJS:=$(addprefix $(REAL_BUILD_DIR)/,$(SRCS:%.cpp=%.o))
DEPS:=$(OBJS:%.o=%.d)

APP_OUTPUT:=$(REAL_DIST_DIR)/$(LIB_SO_NAME)

build: $(APP_OUTPUT) $(LIBMETAWEAR_JAVASCRIPT_PATH)

$(REAL_BUILD_DIR)/%.o: %.cpp
	$(CXX) -MMD -MP -MF "$(@:%.o=%.d)" -c -o $@ $(CXXFLAGS) $<

-include $(DEPS)

$(MODULES_BUILD_DIR):
	mkdir -p $@

$(REAL_DIST_DIR):
	mkdir -p $@

$(OBJS): | $(MODULES_BUILD_DIR)
$(APP_OUTPUT): $(OBJS) | $(REAL_DIST_DIR)
	$(CXX) -o $@ $(LD_FLAGS) $^
# 	ln -sf $(LIB_NAME) $(REAL_DIST_DIR)/$(LIB_SHORT_NAME)
# 	ln -sf $(LIB_SHORT_NAME) $(REAL_DIST_DIR)/$(LIB_SO_NAME)

PUBLISH_NAME:=$(APP_NAME)-$(VERSION).tar
PUBLISH_NAME_ZIP:=$(PUBLISH_NAME).gz

publish: $(DIST_DIR)/$(PUBLISH_NAME_ZIP)

$(DIST_DIR)/$(PUBLISH_NAME_ZIP): $(BUILD_DIR)/$(PUBLISH_NAME)
	gzip --stdout $< > $@

$(BUILD_DIR)/$(PUBLISH_NAME): build
	tar -cf $@ $(BINDINGS_DIR)
	tar -rf $@ --transform 's,$(SOURCE_DIR),include,' $(EXPORT_HEADERS)
	tar -rf $@ -C $(DIST_DIR) .

clean:
	rm -Rf $(BUILD_DIR) $(DIST_DIR)

doc:
	rm -Rf $(DOC_DIR)
	mkdir $(DOC_DIR)
	doxygen Doxyfile

archive:
	make pythonbindings
	make javascriptbindings
	make swiftbindings
	tar -cf metawear_src.tar $(SOURCE_DIR) test/ cppdocs/ Doxyfile LICENSE.md project_version.mk config.mk README.md
	tar -rf metawear_src.tar -C $(BUILD_DIR) bindings

install: $(APP_OUTPUT)
	install $(APP_OUTPUT) /usr/local/lib/$(LIB_SO_NAME)

tag:
	git tag -a $(VERSION)

bindings: 
	$(MAKE) CXX=$(CXX) -C c-binding-generator/ -j4
	$(MAKE) APP_NAME=metawearbinding MODULES=metawear/generator \
        CXXFLAGS="-std=c++11 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -Wall -Werror -Ic-binding-generator/src -Isrc -DMETAWEAR_DLL -DMETAWEAR_DLL_EXPORTS"
	./c-binding-generator/dist/$(CONFIGURATION)/bin/$(MACHINE)/cbinds --cxx-flags "-std=c++11 -I. -Isrc -DMETAWEAR_DLL -DMETAWEAR_DLL_EXPORTS" \
        --generator-lib dist/$(CONFIGURATION)/lib/$(MACHINE)/libmetawearbinding.$(EXTENSION).$(VERSION_MAJOR) \
        --generator-creator $(CREATOR) -f $(BUILD_DIR)/metawear.h -o $(OUTPUT)

export PYTHONPATH=$(BUILD_DIR)/bindings/python/
export METAWEAR_LIB_SO_NAME=$(APP_OUTPUT)

pythonbindings:
	mkdir -p $(BUILD_DIR)/bindings/python/mbientlab/metawear
	$(MAKE) bindings CREATOR=createPythonGenerator OUTPUT=$(PYTHON_BINDINGS)

javascriptbindings:
	mkdir -p $(BUILD_DIR)/bindings/javascript
	$(MAKE) bindings CREATOR=createJavaScriptGenerator OUTPUT=$(JAVASCRIPT_BINDINGS)
	$(MAKE) $(LIBMETAWEAR_JAVASCRIPT_PATH)

swiftbindings:
	echo $(MASTER_HEADERS) > $(BUILD_DIR)/metawear.h
	mkdir -p $(BUILD_DIR)/bindings/swift
	$(MAKE) bindings CREATOR=createSwiftGenerator OUTPUT=$(SWIFT_BINDINGS)

test: build
	make pythonbindings
	python3 -m unittest discover -s test

test-debug: build
	make pythonbindings
	$(DBG) python3

$(LIBMETAWEAR_JAVASCRIPT_PATH):
	@echo "module.exports = '$(abspath $(METAWEAR_LIB_SO_NAME))';" > $(LIBMETAWEAR_JAVASCRIPT_PATH)
