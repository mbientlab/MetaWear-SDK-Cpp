.. highlight:: cpp

SPI
===
SPI is also known as Serial Peripheral Interface, it is a synchronous serial data protocol that acts as an interface bus which operates at full-duplex where data can be sent and received simultaneously. 

SPI is one of the most popular communication peripheral used by microcontrollers to send data to one or more peripheral devices like SD cards and sensors quickly over short distances.

The SPI module allows users to utilize the SPI bus. If you want to add a sensor to the MetaWear board that communicates with SPI, this is possible using the SPI module.

Functions are defined in the `spi.h <https://mbientlab.com/docs/metawear/cpp/latest/spi_8h.html>`_ header file.  

Data Signal
-----------
I2C data signals are retrieved by calling 
`mbl_mw_spi_get_data_signal <https://mbientlab.com/docs/metawear/cpp/latest/spi_8h.html#aa62819b709527840a672a2cc0a242e8d>`_.  You will need to pass 
two parameters: 

* Length variable that sets how many bytes the signal is expected to receive
* An unique ID identifying the signal

If the id value has already been used, the length parameter will be ignored and the previously created signal will be returned.  ::

    #include "metawear/sensor/spi.h"

    // create and get a datasignal for 5 bytes of spi data
    auto spi_signal= mbl_mw_spi_get_data_signal(board, 5, 0xe);

Read
----
To read SPI data, use the 
`mbl_mw_datasignal_read_with_parameters <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a71391d5862eb18327ce2aaaac4a12159>`_ 
function with the parameters set by the `MblMwSpiParameters <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwSpiParameters.html>`_ struct.  
When reading SPI data, the byte array pointed to by the ``data`` field will be written on the bus before reading.  

SPI data is always returned as a byte array.  ::

    #include "metawear/core/datasignal.h"
    #include "metawear/sensor/spi.h"

    mbl_mw_datasignal_subscribe(spi_signal, [](const MblMwData* data) -> void {
        printf("SPI read= [0x%02x", ((uint8_t*) data->value)[0]);
        for(uint8_t i= 1; i < data->length; i++) {
            printf(", 0x%02x", ((uint8_t*) data->value)[i]);
        }
        printf("]\n");
    });

    // Read 5 bytes from the BMI160 starting at register 0x5a (INT_LOWHIGH)
    MblMwSpiParameters read_params= { MBL_MW_SPI_MODE_3, MBL_MW_SPI_FREQUENCY_8_MHZ, 
            {0xda}, 1, 0xa, 0, 0xb, 7, 0, 1};
    mbl_mw_datasignal_read_with_parameters(spi_signal, &read_params);

Write
-----
Writing data through the SPI bus is handled with the 
`mbl_mw_spi_write <https://mbientlab.com/docs/metawear/cpp/latest/spi_8h.html#acd7af3167e03424cc35ffc7a6a29d39e>`_ function.  The same 
`MblMwSpiParameters <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwSpiParameters.html>`_ struct is used to wrap the required 
parameters into one variable.
