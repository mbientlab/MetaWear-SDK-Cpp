class BaroBmp280Oversampling(object):
    SKIP= 0,
    ULTRA_LOW_POWER= 1
    LOW_POWER= 2
    STANDARD= 3
    HIGH= 4
    ULTRA_HIGH= 5

class BaroBmp280IirFilter(object):
    OFF= 0
    AVG_2= 1
    AVG_4= 2
    AVG_8= 3
    AVG_16= 4

class BaroBmp280Standby(object):
    STANDBY_0_5MS= 0
    STANDBY_62_5MS= 1
    STANDBY_125MS= 2
    STANDBY_250MS= 3
    STANDBY_500MS= 4
    STANDBY_1000MS= 5
    STANDBY_2000MS= 6
    STANDBY_4000MS= 7
