from ctypes import c_int, Structure

class AccBmi160Config(Structure):
    _fields_= [
        ("odr", c_int),
        ("range", c_int)
    ]

class AccBmi160Odr(object):
    ODR_0_78125HZ= 0
    ODR_1_5625HZ= 1
    ODR_3_125HZ= 2
    ODR_6_25HZ= 3
    ODR_12_5HZ= 4
    ODR_25HZ= 5
    ODR_50HZ= 6
    ODR_100HZ= 7
    ODR_200HZ= 8
    ODR_400HZ= 9
    ODR_800HZ= 10
    ODR_1600HZ= 11

class AccBmi160Range(object):
    FSR_2G= 0
    FSR_4G= 1
    FSR_8G= 2
    FSR_16G= 3
