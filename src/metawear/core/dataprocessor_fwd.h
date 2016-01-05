#pragma once

#ifdef __cplusplus
struct MblMwDataProcessor; 
#else
typedef struct MblMwDataProcessor MblMwDataProcessor;
#endif

typedef void (*MblMwFnDataProcessor)(MblMwDataProcessor* new_processor);
