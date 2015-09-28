M_NAME:=metawear
M_SRCS:=debug.cpp led.cpp neopixel.cpp haptic.cpp ibeacon.cpp switch.cpp sensor.cpp accelerometer_mma8452q.cpp \
accelerometer_bmi160.cpp barometer_bmp280.cpp gyro_bmi160.cpp ambientlight_ltr329.cpp
M_HEADERS:=debug.h led.h neopixel.h haptic.h ibeacon.h switch.h sensor.h sensor_defs.h accelerometer_mma8452q.h \
accelerometer_bmi160.h barometer_bmp280.h gyro_bmi160.h dataprocessor.h ambientlight_ltr329.h types.h dllmarker.h

SRCS:= $(SRCS) $(addprefix $(SOURCE_DIR)/$(M_NAME)/cpp/,$(M_SRCS))
EXPORT_HEADERS:=$(EXPORT_HEADERS) $(addprefix $(M_NAME)/,$(M_HEADERS))
