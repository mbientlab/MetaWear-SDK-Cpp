.. highlight:: cpp

I2C
===
The I2C module allows you to directly communicate with a sensor via the I2C bus.  I2C functions are defined in the 
`i2c.h <https://mbientlab.com/docs/metawear/cpp/latest/i2c_8h.html>`_ header file.

Data Signal
-----------
I2C data signals are retrieved by calling 
`mbl_mw_i2c_get_data_signal <https://mbientlab.com/docs/metawear/cpp/latest/i2c_8h.html#af95de21e3550c0fa483a0f5eb228a391>`_.  You will need to pass 
two parameters: 

* Length variable that sets how many bytes the signal is expected to receive
* An unique ID identifying the signal

If the id value has already been used, the length parameter will be ignored and the previously created signal will be returned.  ::

    #include "metawear/sensor/i2c.h"

    // create and get a datasignal for 1 byte of i2c data
    auto i2c_signal= mbl_mw_i2c_get_data_signal(board, 1, 0xa);

Read
----
To read I2C data, use the 
`mbl_mw_datasignal_read_with_parameters <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a71391d5862eb18327ce2aaaac4a12159>`_ 
function with the parameters set by the `MblMwI2cReadParameters <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwI2cReadParameters.html>`_ 
struct.  ::

    #include "metawear/core/datasignal.h"
    #include "metawear/sensor/i2c.h"

    mbl_mw_datasignal_subscribe(i2c_signal, [](const MblMwData* data) -> void {
        printf("WHO_AM_I= %x", ((uint8_t*) data->value)[0]);
    });

    MblMwI2cReadParameters read_params= {0x1c, 0xd};
    mbl_mw_datasignal_read_with_parameters(i2c_signal, &read_params);

Write
-----
Writing data through the I2C bus is handled with the 
`mbl_mw_i2c_write <https://mbientlab.com/docs/metawear/cpp/latest/i2c_8h.html#a484a0f6338a2d90eb9167283c6859165>`_ function.  ::

    #include "metawear/sensor/i2c.h"
    
    void i2c_write(MblMwMetaWearBoard* board) {
        // Write to the ctrl_meas register on the RPro/CPro barometer
        uint8_t ctrl_meas= 0x37;
        mbl_mw_i2c_write(board, 0x77, 0xf4, &ctrl_meas, 1);
    }

