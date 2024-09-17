#include "../include/DataStruct.hpp"
#include <iostream>

long lstTm = 0;
// So we have to measure time from last

float pitchInProgress = 1500;

float akYaw = 0.1;//0.5;//0.25//1.0;
float yawMaxAdd = 100;
float akRoll = 1.0; //0.5
float rollMaxAdd = 200;
float pitchMax = 1700;
float pitchK = 1.0;
float pitchAdd = 50;
float throttAdd = 10;


long currentTimeMillis() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
			   std::chrono::system_clock::now().time_since_epoch())
		.count();
}


extern "C" {


void init(DataStruct* dta)
{
	std::cout << "external AP init called!\n";
}

void process(DataStruct* dta){
		
	int pitchOnRC = dta->RCChannelsRaw[dta->conf->cPitch];
	
	// count dx, dy
	int dx = dta->bbox->x + dta->bbox->width / 2 - (*(dta->CX) + *(dta->WX)/2);
	int dy = dta->bbox->y + dta->bbox->height / 2 - (*(dta->CY) + *(dta->WY)/2);
	
	int yawAdd = (int)(dx * akYaw);
	// Make shure abs yaw not exeeds limits
	if (yawAdd > yawMaxAdd) yawAdd = yawMaxAdd;
	if (yawAdd < -yawMaxAdd) yawAdd = -yawMaxAdd;
	
	//int yaw = MID_VAL + yawAdd;
	int yaw = dta->conf->chM + yawAdd;
	
	// roll
	int rollAdd = (int)(dx * akRoll);
	
	if (lstTm > 0) 
		rollAdd = (int)dta->rollPid->compute(0, -dx, (currentTimeMillis()-lstTm)/1000.0);
	
	if (rollAdd > rollMaxAdd) rollAdd = rollMaxAdd;
	if (rollAdd < -rollMaxAdd) rollAdd = -rollMaxAdd;
	
	//int roll = MID_VAL + rollAdd;
	int roll = dta->conf->chM + rollAdd;
	
	// Making pitch setting not immidiate, but during pitch set timeout
	int pitch = dta->RCChannelsRaw[dta->conf->cPitch];
	pitch = dta->RCPitchLast;
	if (pitch < dta->conf->pitchCruse && !dta->settingPitchInProgress && !dta->pitchIsSet)
	{
		pitchInProgress = pitch;
		dta->settingPitchInProgress = true;
		dta->pitchIsSet = false;
	}
	else if (dta->settingPitchInProgress && pitchInProgress < dta->conf->pitchCruse){
		long millis = currentTimeMillis() - lstTm;
		pitchAdd = millis * dta->conf->pitchAddSpeed;
		pitchInProgress += pitchAdd;
		if (pitchInProgress >= dta->conf->pitchCruse){
			pitchInProgress = dta->conf->pitchCruse;
			dta->settingPitchInProgress = false;
			dta->pitchIsSet = true;
		}
		pitch = pitchInProgress;
	}
	
	if (dta->pitchIsSet) pitch = pitchInProgress;
	
	int thrHoov = dta->RCThrottleLast; // Set last pilot input throttle
	
	if (thrHoov < dta->conf->thrHoovNominal) thrHoov = dta->conf->thrHoovNominal;
	
	int throttle = dta->RCChannelsRaw[dta->conf->cThrottle];
	float thrMaxAdd = 1000;
	float thrMinAdd = -1000;
	float akThr = 1.0;
	int thrAdd = 0;
	if (lstTm > 0 
			//&& dta->assignRCStarted
		)
	{
		// IN theory, to hold ALTITUDE, we just have to replace dy with DesiredAlt (100) - LastAlt
		thrAdd = (int)dta->thrPid->compute(0, dy, (currentTimeMillis()-lstTm)/1000.0);
		// On BTFL, altitude is set to 0 after arming... before arming it seems to set by alt above the sea level)
		//thrAdd = (int)thrPID->compute(0, -(100 - fcReader.lastAlt), (currentTimeMillis()-lstTm)/1000.0);
	}
	if (thrAdd > thrMaxAdd) thrAdd = thrMaxAdd;
	if (thrAdd < thrMinAdd) thrAdd = thrMinAdd;
	throttle = thrHoov + thrAdd;
	
	int thrMax = 2000;
	if (throttle < 990) throttle = 990;
	if (throttle > thrMax) throttle = thrMax;
		
	//std::cout << "set dta->RCChannelsOverride\n"; 
	dta->RCChannelsOverride[dta->conf->cRoll] = roll;
	dta->RCChannelsOverride[dta->conf->cPitch] = pitch;
	dta->RCChannelsOverride[dta->conf->cYaw] = yaw;
	dta->RCChannelsOverride[dta->conf->cThrottle] = throttle;
	
	lstTm = currentTimeMillis();
	
}

} // extern "C"
