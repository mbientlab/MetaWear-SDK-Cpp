#include "anonymous_datasignal_private.h"
#include "metawear/core/anonymous_datasignal.h"

MblMwAnonymousDataSignal::~MblMwAnonymousDataSignal() { }

void mbl_mw_anonymous_datasignal_subscribe(MblMwAnonymousDataSignal* signal, void *context, MblMwFnData data_handler) {
    signal->subscribe(context, data_handler);
}

const char* mbl_mw_anonymous_datasignal_get_identifier(const MblMwAnonymousDataSignal* signal) {
    return signal->get_identifier();
}