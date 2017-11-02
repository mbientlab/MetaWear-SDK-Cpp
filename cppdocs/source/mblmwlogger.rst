.. highlight:: cpp

Logger
======
Loggers record data from a data signal and are represented by the 
`MblMwDataLogger <https://mbientlab.com/docs/metawear/cpp/0/logging__fwd_8h.html#a84a99b569b691df5017c03721645b49d>`_ struct.  Create an 
``MblMwDataLogger`` object by calling 
`mbl_mw_datasignal_log <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#aa7ec82a61e31616ff2eaedb0a96160d8>`_ with the data signal 
you want to log.  If successful, the callback function will be executed with a  
`MblMwDataLogger <https://mbientlab.com/docs/metawear/cpp/0/logging__fwd_8h.html#a84a99b569b691df5017c03721645b49d>`_ pointer and if creating the 
logger failed, a null pointer will be returned.  ::

    #include "metawear/core/datasignal.h"
    #include "metawear/core/logging_fwd.h"
    #include "metawear/sensor/multichanneltemperature.h"
    
    auto temp_signal = mbl_mw_multi_chnl_temp_get_temperature_data_signal(board, 0);
    mbl_mw_datasignal_log(temp_signal, [](MblMwDataLogger* logger) -> void {
        if (logger != nullptr) {
            printf("logger ready\n");
        } else {
            printf("Failed to create the logger\n");
        }
    });

MblMwDataLogger objects only interact with the specific data signal, they do not control the logging features.  Logging control functions are detailed in the :doc:`logging` section.

ID
--
MblMwDataLogger objects are identified by a numerical id; you can retrieve the id by calling 
`mbl_mw_logger_get_id <https://mbientlab.com/docs/metawear/cpp/0/logging_8h.html#ab32e4ae06e057cbb0180558ef8ec8165>`_.  The id is used to 
retrieve existing loggers from the API with the 
`mbl_mw_logger_lookup_id <https://mbientlab.com/docs/metawear/cpp/0/logging_8h.html#a1b95ca107021c1e8f6ddaef0fbc85c4b>`_ function.

Handling Data
-------------
Like a data signal, you can subscribe to an MblMwDataLogger to process the downloaded data.  Call 
`mbl_mw_logger_subscribe <https://mbientlab.com/docs/metawear/cpp/0/logging_8h.html#ac1fa6f2a678f61d86ccc80b092e8c098>`_ to attach a callback 
function to the MblMwDataLogger which handles all received data.  ::

    void logger_subscribe(MblMwDataLogger* temp_logger) {
        mbl_mw_logger_subscribe(temp_logger, [](const MblMwData* data) -> void {
            printf("temperature= %.3fC\n", *((float*) data->value));
        });
    }

Removal
-------
When you no longer want to log the values from a data signal, call 
`mbl_mw_logger_remove <https://mbientlab.com/docs/metawear/cpp/0/logging_8h.html#a8877b9a3f6c8571c41c21cda4a9c90cb>`_ to remove the logger.
