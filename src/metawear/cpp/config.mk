M_NAME:=metawear

SRCS:= $(SRCS) $(shell find $(SOURCE_DIR)/$(M_NAME)/cpp -name \*.cpp)
EXPORT_HEADERS:=$(EXPORT_HEADERS) $(shell find $(SOURCE_DIR)/$(M_NAME) -maxdepth 1 -name \*.h)
