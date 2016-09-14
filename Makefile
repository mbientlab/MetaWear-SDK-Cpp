.PHONY: build clean test doc publish

include config.mk
include project_version.mk

EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)
COMMA:=,

MODULES_SRC_DIR= $(addsuffix /cpp, $(addprefix $(SOURCE_DIR)/, $(MODULES)))
SRCS:=$(foreach src_dir, $(MODULES_SRC_DIR), $(shell find $(src_dir) -name \*.cpp))
EXPORT_HEADERS:=$(foreach module, $(addprefix $(SOURCE_DIR)/, $(MODULES)), $(shell find $(module) -maxdepth 1 -name \*.h))

CXXFLAGS+=-std=c++11 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -Wall -Werror -I$(SOURCE_DIR) -DMETAWEAR_DLL -DMETAWEAR_DLL_EXPORTS
LD_FLAGS:=-s -shared -Wl,

ifeq ($(CONFIGURATION),debug)
    APP_NAME:=$(APP_NAME)_d
    CXXFLAGS+=-g
else ifeq ($(CONFIGURATION),release)
    CXXFLAGS+=-O3
else
    $(error Invalid value for "CONFIGURATION", must be 'release' or 'debug')
endif

ifneq ($(KERNEL),Darwin)
    EXTENSION:=so
    LIB_SO_NAME:=lib$(APP_NAME).so
    LD_FLAGS:=$(LD_FLAGS)--soname
else
    EXTENSION:=dylib
    LD_FLAGS:=-dynamiclib $(LD_FLAGS)-install_name
endif
LIB_SO_NAME:=lib$(APP_NAME).$(EXTENSION)
LIB_SHORT_NAME:=$(LIB_SO_NAME).$(VERSION_MAJOR)
LIB_NAME:=$(LIB_SO_NAME).$(VERSION)
LD_FLAGS:=$(LD_FLAGS),$(LIB_SHORT_NAME)

ifeq ($(MACHINE),x86)
    CXXFLAGS+=-m32
    LD_FLAGS+=-m32
else ifeq ($(MACHINE),x64)
    CXXFLAGS+=-m64
    LD_FLAGS+=-m64
else ifeq ($(MACHINE),arm)
    CXXFLAGS+=-marm
    LD_FLAGS+=-marm
else
    $(error Unrecognized "MACHINE" value, use 'x86', 'x64', or 'arm')
endif

REAL_DIST_DIR:=$(DIST_DIR)/$(CONFIGURATION)/lib/$(MACHINE)
REAL_BUILD_DIR:=$(BUILD_DIR)/$(MACHINE)/$(CONFIGURATION)
MODULES_BUILD_DIR:=$(addprefix $(REAL_BUILD_DIR)/, $(MODULES_SRC_DIR))

OBJS:=$(addprefix $(REAL_BUILD_DIR)/,$(SRCS:%.cpp=%.o))
DEPS:=$(OBJS:%.o=%.d)

APP_OUTPUT:=$(REAL_DIST_DIR)/$(LIB_NAME)

build: $(MODULES_BUILD_DIR) $(REAL_DIST_DIR) $(APP_OUTPUT)

$(REAL_BUILD_DIR)/%.o: %.cpp
	$(CXX) -MMD -MP -MF "$(@:%.o=%.d)" -c -o $@ $(CXXFLAGS) $<

-include $(DEPS)

$(MODULES_BUILD_DIR):
	mkdir -p $@

$(REAL_DIST_DIR):
	mkdir -p $@

$(APP_OUTPUT): $(OBJS)
	$(CXX) -o $@ $(LD_FLAGS) $^
	ln -sf $(LIB_NAME) $(REAL_DIST_DIR)/$(LIB_SHORT_NAME)
	ln -sf $(LIB_SHORT_NAME) $(REAL_DIST_DIR)/$(LIB_SO_NAME)

PUBLISH_NAME:=$(APP_NAME)-$(VERSION).tar
PUBLISH_NAME_ZIP:=$(PUBLISH_NAME).gz

publish: $(DIST_DIR)/$(PUBLISH_NAME_ZIP)

$(DIST_DIR)/$(PUBLISH_NAME_ZIP): $(BUILD_DIR)/$(PUBLISH_NAME)
	gzip --stdout $< > $@

$(BUILD_DIR)/$(PUBLISH_NAME): build
	tar -cf $@ $(WRAPPER_DIR)
	tar -rf $@ --transform 's,$(SOURCE_DIR),include,' $(EXPORT_HEADERS)
	tar -rf $@ -C $(DIST_DIR) .

clean:
	rm -Rf $(BUILD_DIR) $(DIST_DIR) $(EXT_LIBS_DIR)

doc:
	rm -Rf $(DOC_DIR)
	mkdir $(DOC_DIR)
	doxygen Doxyfile

export PYTHONPATH=$(WRAPPER_DIR)/python
export METAWEAR_LIB_SO_NAME=$(APP_OUTPUT)

test: build
	python3 -m unittest discover -s test
