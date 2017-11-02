#include "anonymous_datasignal_private.h"
#include "metawear/core/anonymous_datasignal.h"

MblMwAnonymousDataSignal::~MblMwAnonymousDataSignal() { }

void mbl_mw_anonymous_datasignal_subscribe(MblMwAnonymousDataSignal* signal, MblMwFnData data_handler) {
    signal->subscribe(data_handler);
}

char* mbl_mw_anonymous_datasignal_get_identifier(const MblMwAnonymousDataSignal* signal) {
    return signal->get_identifier();
}