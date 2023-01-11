#include "common.h"

using std::string;
using std::unordered_map;

unordered_map<string, string> SHORTENED_PREFIXES = {
    { "MblMwComparatorOperation", "MBL_MW_COMPARATOR_OP_" },
    { "MblMwDataTypeId", "MBL_MW_DT_ID_" },
    { "MblMwMetaWearRProChannel", "MBL_MW_METAWEAR_RPRO_CHANNEL_" },
    { "MblMwMathOperation", "MBL_MW_MATH_OP_"},
    { "MblMwAlsLtr329IntegrationTime", "MBL_MW_ALS_LTR329_TIME_" },
    { "MblMwAlsLtr329MeasurementRate", "MBL_MW_ALS_LTR329_RATE_" },
    { "MblMwTimeMode", "MBL_MW_TIME_" },
    { "MblMwGattCharWriteType", "MBL_MW_GATT_CHAR_WRITE_" },
    { "MblMwWhitelistFilter", "MBL_MW_WHITELIST_FILTER_"},
    { "MblMwAccBoschDoubleTapWindow", "MBL_MW_ACC_BOSCH_DOUBLE_TAP_WINDOW_"}
};

string createPrefix(const string& in) { 
    string prefix;
    bool first = true;

    for(auto ch: in) {
        if (!first && isupper(ch) && ch != 'W') {
            prefix+= "_";
        }
        prefix+= toupper(ch);
        first = false;
    }
    prefix+= "_";

    return prefix;
}