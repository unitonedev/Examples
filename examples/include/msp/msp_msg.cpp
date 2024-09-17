#include "msg_msg.hpp"

{
    std::string val;
    if(flag & (1 << 2)) val += "ARMED ";
    if(flag & (1 << 3)) val += "WAS_EVER_ARMED ";
    if(flag & (1 << 7)) val += "ARMING_DISABLED_FAILSAFE_SYSTEM ";
    if(flag & (1 << 8)) val += "ARMING_DISABLED_NOT_LEVEL ";
    if(flag & (1 << 9)) val += "ARMING_DISABLED_SENSORS_CALIBRATING ";
    if(flag & (1 << 10)) val += "ARMING_DISABLED_SYSTEM_OVERLOADED ";
    if(flag & (1 << 11)) val += "ARMING_DISABLED_NAVIGATION_UNSAFE ";
    if(flag & (1 << 12)) val += "ARMING_DISABLED_COMPASS_NOT_CALIBRATED ";
    if(flag & (1 << 13)) val += "ARMING_DISABLED_ACCELEROMETER_NOT_CALIBRATED ";
    if(flag & (1 << 14)) val += "ARMING_DISABLED_ARM_SWITCH ";
    if(flag & (1 << 15)) val += "ARMING_DISABLED_HARDWARE_FAILURE ";
    if(flag & (1 << 16)) val += "ARMING_DISABLED_BOXFAILSAFE ";
    if(flag & (1 << 17)) val += "ARMING_DISABLED_BOXKILLSWITCH ";
    if(flag & (1 << 18)) val += "ARMING_DISABLED_RC_LINK ";
    if(flag & (1 << 19)) val += "ARMING_DISABLED_THROTTLE ";
    if(flag & (1 << 20)) val += "ARMING_DISABLED_CLI ";
    if(flag & (1 << 21)) val += "ARMING_DISABLED_CMS_MENU ";
    if(flag & (1 << 22)) val += "ARMING_DISABLED_OSD_MENU ";
    if(flag & (1 << 23)) val += "ARMING_DISABLED_ROLLPITCH_NOT_CENTERED ";
    if(flag & (1 << 24)) val += "ARMING_DISABLED_SERVO_AUTOTRIM ";
    if(flag & (1 << 25)) val += "ARMING_DISABLED_OOM ";
    if(flag & (1 << 26)) val += "ARMING_DISABLED_INVALID_SETTING ";
    return val;
}
