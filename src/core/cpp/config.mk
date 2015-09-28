M_NAME:=core
M_SRCS:=
M_HEADERS:=dllmarker.h

SRCS:= $(SRCS) $(addprefix $(SOURCE_DIR)/$(M_NAME)/cpp/,$(M_SRCS))
EXPORT_HEADERS:=$(EXPORT_HEADERS) $(addprefix $(M_NAME)/,$(M_HEADERS))
