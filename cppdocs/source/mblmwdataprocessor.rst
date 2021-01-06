.. highlight:: cpp

Data Processor
==============
Data processors are functions in the firmware that filter or transform sensor data and are represented by the 
`MblMwDataProcessor <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor__fwd_8h.html#a7bbdad259a1328a17a634de3035c42e3>`_ struct.  

In the C++ API, data processors can be thought of as a data signal whose data is produced by the on-board data processor.  As such, a 
`MblMwDataProcessor <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor__fwd_8h.html#a7bbdad259a1328a17a634de3035c42e3>`_ can be safely 
typecasted to a `MblMwDataSignal <https://mbientlab.com/docs/metawear/cpp/latest/datasignal__fwd_8h.html#a1ce49f0af124dfa7984a59074c11e789>`_.

.. list-table::
   :header-rows: 1

   * - Processor Id
     - Name
     - Functionality
   * - 0x01
     - Passthrough
     - Allow a specific # of data (samples) through
   * - 0x02
     - Accumulator
     - Running sum of data
   * - 0x02
     - Counter
     - Keeps a tally of how many times it is called
   * - 0x03
     - Averager / Low Pass Filter
     - Computes a running average of the data
   * - 0x03
     - High Pass Filter 
     - Compute the difference of the current value from a running average of the previous N samples
   * - 0x06
     - Comparator
     - Removes data that do not satisfy the comparison operation
   * - 0x07
     - RMS
     - Computes the root mean square over multi component data
   * - 0x07
     - RSS
     - Computes the root sum square, or vector magnitude, over multi component data
   * - 0x08
     - Time
     - Only allows data to pass at fixed intervals
   * - 0x09
     - Math
     - Performs arithmetic or logical operations on the data
   * - 0x0a
     - Sample
     - Only allowing data through once it has collected a set number of samples
   * - 0x0b
     - Pulse
     - Detects and quantifies a pulse over a set of data
   * - 0x0c
     - Delta
     - Computes the dif between 2 successive data values and only allows data through that creates a diff > threshold
   * - 0x0d
     - Threshold
     - Only allows data through that crosses a boundary
   * - 0x0f
     - Buffer
     - Captures input data which can be read at a later time
   * - 0x10
     - Packer
     - Combines multiple data samples into 1 BLE packet 
   * - 0x11
     - Accounter
     - Adds additional information to the BTLE packet
   * - 0x1b
     - Fuser
     - Combine data from multiple sources into 1
 

This section will focus on the 
`MblMwDataProcessor <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor__fwd_8h.html#a7bbdad259a1328a17a634de3035c42e3>`_ struct.  Details on 
supported processors and how to create them are covered in the :doc:`dataprocessor` section.

ID
--
Data processors are identified by a unique numerical ID; as identified in the table above.

You can retrieve this id by calling 
`mbl_mw_dataprocessor_get_id <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor_8h.html#a57d4952e5ffe511cd7895ff2bf2ab64e>`_.  

The data processor ID is used to lookup a previously created MblMwDataProcessor object with the 
`mbl_mw_dataprocessor_lookup_id <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor_8h.html#ada480683db69acc464034923a67c4ae4>`_ function.  

The object is needed to remove/delete processors (see section below).

The object is useful as it can be used as the input to another processor. As such, it is possible to combine processors together for all sorts of functionality.

A high pass filter on the accelerometer data can remove noise which can then be set as the input to an RMM to combine the x,y,z axis data into a signal signal. This can further be set as the input to a comparator or threshold detector to determine if the accelerometer experienced a shock higher than 4Gs.

::

    rssFunction() {
        // #3 - Average 4 vector values into 1
        mbl_mw_dataprocessor_average_create(pointer, 4, context, otherFunction))
        // #4 - Could keep adding processors or subscribe to the final data signal in otherFunction()
    }
       
    // #1 - Get the accelerometer signal
    accel_signal = mbl_mw_acc_get_acceleration_data_signal(board)
    // #2 - Calculate the vector of the x,y,x 
    mbl_mw_dataprocessor_rss_create(accel_signal, rssFunction())

State
-----
Some processors have an internal state that can be read and modified; the internal state is treated as a readable
`MblMwDataSignal <https://mbientlab.com/docs/metawear/cpp/latest/datasignal__fwd_8h.html#a1ce49f0af124dfa7984a59074c11e789>`_.  

In this example, a buffer process is used. The state of the buffer processor is the data in the buffer:

::

    #include "metawear/core/datasignal.h"
    #include "metawear/processor/dataprocessor.h"
    
    // Assume input processor is a Buffer processor
    void subscribe_state_signal(MblMwDataProcessor* buffer_processor) {
        auto state_signal = mbl_mw_dataprocessor_get_state_data_signal(buffer_processor);
        mbl_mw_datasignal_subscribe(state_signal, data_printer);
        mbl_mw_datasignal_read(state_signal);
    }

The state of the counter processor is the count itself.

::

    // Previously setup the counter in function
    processor = mbl_mw_dataprocessor_lookup_id(board, 2)
    processor_state = mbl_mw_dataprocessor_get_state_data_signal(processor)
    mbl_mw_datasignal_subscribe(processor_state, sensor_data_handler)
       

Removal
-------
Removing a processor is handled by calling 
`mbl_mw_dataprocessor_remove <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor_8h.html#ab5f75966b3887ce11a7730419118e03f>`_.  

When a processor is removed, all processors that consume its output will also be removed. ::

    #include "metawear/core/datasignal_fwd.h"
    #include "metawear/processor/dataprocessor.h"
    #include "metawear/processor/math.h"
    
    void remove_processors(MblMwDataSignal* temp_signal) {
        static MblMwDataProcessor* first_processor= nullptr;
    
        static auto add_32_created = [](MblMwDataProcessor* processor) -> void {
            // Removes all 3 processors in the chain
            mbl_mw_dataprocessor_remove(first_processor);
        };
        static auto div_10_created = [](MblMwDataProcessor* processor) -> void {
            mbl_mw_dataprocessor_math_create((MblMwDataSignal*) processor, MBL_MW_MATH_OP_ADD, 32.f, 
                    add_32_created);
        };
        static auto mul_18_created = [](MblMwDataProcessor* processor) -> void {
            first_processor = processor;
            mbl_mw_dataprocessor_math_create((MblMwDataSignal*) processor, MBL_MW_MATH_OP_DIVIDE, 
                    10.f, div_10_created);
        };
        mbl_mw_dataprocessor_math_create(temp_signal, MBL_MW_MATH_OP_MULTIPLY, 18.f, mul_18_created);
    }

