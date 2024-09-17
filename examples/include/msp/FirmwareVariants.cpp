#include "FirmwareVariants.hpp"
std::string firmwareVariantToString(FirmwareVariant variant) {
    std::string var;
    switch(variant) {
    case FirmwareVariant::MWII:
        var = "MWII";
        break;
    case FirmwareVariant::BAFL:
        var = "BAFL";
        break;
    case FirmwareVariant::BTFL:
        var = "BTFL";
        break;
    case FirmwareVariant::CLFL:
        var = "CLFL";
        break;
    case FirmwareVariant::INAV:
        var = "INAV";
        break;
    case FirmwareVariant::RCFL:
        var = "RCFL";
        break;
    default:
        var = "NONE";
    }
    return var;
}
