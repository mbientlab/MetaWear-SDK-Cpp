.PHONY: build clean test doc

include config.mk
include project_version.mk

WRAPPER_DIR=wrapper
SRCS:=
EXPORT_HEADERS:=

MODULES_SRC_DIR= $(addsuffix /cpp, $(addprefix $(SOURCE_DIR)/, $(MODULES)))
MODULES_CONFIG:= $(addsuffix /config.mk, $(MODULES_SRC_DIR))

include $(MODULES_CONFIG)

LIB_SO_NAME:=lib$(APP_NAME).so
LIB_SHORT_NAME:=$(LIB_SO_NAME).$(VERSION_MAJOR)
LIB_NAME:=$(LIB_SO_NAME).$(VERSION)

CXXFLAGS+=-std=c++11 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -Wall -Werror -I$(SOURCE_DIR) -DMETAWEAR_DLL -DMETAWEAR_DLL_EXPORTS
LD_FLAGS:=-s -shared -Wl,--soname,$(LIB_SHORT_NAME)

ifeq ($(PLATFORM),x86)
    CXXFLAGS+=-m32
    LD_FLAGS+=-m32
else ifeq ($(PLATFORM),x64)
    CXXFLAGS+=-m64
    LD_FLAGS+=-m64
else
    $(error Unrecognized PLATFORM value, use x86 or x64)
endif

ifeq ($(CONFIGURATION),debug)
    APP_NAME:=$(APP_NAME)_d
    CXXFLAGS+=-g
else ifeq ($(CONFIGURATION),release)
    CXXFLAGS+=-O3
else
    $(error Invalid value for "CONFIGURATION", must be 'release' or 'debug')
endif

REAL_DIST_DIR:=$(DIST_DIR)/$(CONFIGURATION)/lib/$(PLATFORM)
REAL_BUILD_DIR:=$(BUILD_DIR)/$(PLATFORM)/$(CONFIGURATION)
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
