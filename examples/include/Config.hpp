#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <string>
#include <iostream>

struct ConfigStruct{
	
	std::string cameraName = "";
	
	int cRoll = 0;   // Normal order is like so!!!
	int cPitch = 1;
	int cYaw = 2;
	int cThrottle = 3;
	
	// This values are got by asking firmware!!!!
	// They are for writing rc commands
	// all that reading are upper!!! and they seems to be in constant order for all FCs (rol, pit, yaw, thr)
	// but these downwards can change from FC to FC!!!
	int wrcRoll = 0;
	int wrcPitch = 1;
	int wrcYaw = 2;
	int wrcThrottle = 3;

	
	int chM = 1500;
	int ch_dM = 50;
	int chL = 1100;
	int chH = 1900;
	
	int thrHoovNominal = 1300;
	
	int pitchCruse = 1700;
	float pitchSetTimeout = 2000; // Time to raise pitch from 1500 to pitchCruse 
	float pitchAddSpeed = (pitchCruse - 1500 + 0.0) / (pitchSetTimeout + 0.0);
	
	int throttleMax = 2000;
	int throttleMin = 990;
	
	int sleepWriteRCmillis = 40;
	int sleepBBmillis = 50;
	
	
	bool altitudeRequired = true;
	bool attitudeRequired = true;
	
	bool autoPilotIsOn = true;
	std::string externalAutoPilotPath = "";
	
	bool trackerIsOn = true;
	std::string externalTrackerPath = "";
	int initialBoxWidth = 50;
	int initialBoxHeight = 50;
	int changeBoxSizeRCchannel = 10;
	
	bool streamerIsOn = true;
	std::string externalStreamerPath = "";
	std::string streamerPort = "5555";

	long streamerSleepPeriod = 50;
	
	std::string externalOverallPath = "";
	
	
};

#endif