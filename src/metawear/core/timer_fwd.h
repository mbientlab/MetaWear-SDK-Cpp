#pragma once

#ifdef __cplusplus
struct MblMwTimer; 
#else
typedef struct MblMwTimer MblMwTimer;
#endif

typedef void (*MblMwTimerPtr)(MblMwTimer* new_timer);
