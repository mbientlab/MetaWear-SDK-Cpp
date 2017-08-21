.. highlight:: cpp

Data Processor
==============
Data processors are functions in the firmware that filter or transform sensor data and are represented by the 
`MblMwDataProcessor <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor__fwd_8h.html#a7bbdad259a1328a17a634de3035c42e3>`_ struct.  In the 
C++ API, data processors can be thought of as a data signal whose data is produced by the on-board data processor.  As such, a 
`MblMwDataProcessor <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor__fwd_8h.html#a7bbdad259a1328a17a634de3035c42e3>`_ can be safely 
typecasted to a `MblMwDataSignal <https://mbientlab.com/docs/metawear/cpp/latest/datasignal__fwd_8h.html#a1ce49f0af124dfa7984a59074c11e789>`_.

This section will focus on the 
`MblMwDataProcessor <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor__fwd_8h.html#a7bbdad259a1328a17a634de3035c42e3>`_ struct.  Details on 
supported processors and how to create them are covered in the :doc:`dataprocessor` section.

ID
--
Data processors are identified by a unique numerical ID; you can retrieve this id by calling 
`mbl_mw_dataprocessor_get_id <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor_8h.html#a57d4952e5ffe511cd7895ff2bf2ab64e>`_.  The data 
processor ID is used to lookup a previously created MblMwDataProcessor object with the 
`mbl_mw_dataprocessor_lookup_id <https://mbientlab.com/docs/metawear/cpp/latest/dataprocessor_8h.html#ada480683db69acc464034923a67c4ae4>`_ function.  

State
-----
Some processors have an internal state that can be read and modified; the internal state is treated as a readable
`MblMwDataSignal <https://mbientlab.com/docs/metawear/cpp/latest/datasignal__fwd_8h.html#a1ce49f0af124dfa7984a59074c11e789>`_.  ::

    #include "metawear/core/datasignal.h"
    #include "metawear/processor/dataprocessor.h"
    
    // Assume input processor is a Buffer processor
    void subscribe_state_signal(MblMwDataProcessor* buffer_processor) {
        auto state_signal = mbl_mw_dataprocessor_get_state_data_signal(buffer_processor);
        mbl_mw_datasignal_subscribe(state_signal, data_printer);
        mbl_mw_datasignal_read(state_signal);
    }

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

