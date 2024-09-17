#include "../include/DataStruct.hpp"
#include "../include/zmq.hpp"
#include <iostream>
#include <sstream>

zmq::context_t zmqcontext(1);
zmq::socket_t zmqsocket(zmqcontext, ZMQ_PUB);


std::string makeTelemetryString(DataStruct* dta) {
	
	std::stringstream ss;
	ss << "{";
	
	if (dta->currentMode)
		ss << "\"cMode\":" << (*dta->currentMode) << ", ";
	if (dta->bbox)		
		ss << "\"rect\": [" << dta->bbox->x << "," << dta->bbox->y << "," << dta->bbox->width << ","<<dta->bbox->height<<"],";	
	if (dta->real_fps)
		ss << "\"rFPS\":" << (*dta->real_fps) << ", ";
	if (dta->tracker_fps)
		ss << "\"tFPS\":" << (*dta->tracker_fps) << ", ";
	if (dta->RCChannelsRaw)
	{
		ss << "\"RC\": [" << dta->RCChannelsRaw[0] << "," << dta->RCChannelsRaw[1] 
					<< "," << dta->RCChannelsRaw[2] << ","<<dta->RCChannelsRaw[3]<<"]";
		ss << ", ";
	}
	if (dta->RCChannelsOverride)
	{
		ss << "\"RCOver\": [" << dta->RCChannelsOverride[0] << "," << dta->RCChannelsOverride[1] 
					<< "," << dta->RCChannelsOverride[2] << ","<<dta->RCChannelsOverride[3]<<"]";
		ss << ", ";
	}
		
	if (dta->bbHistory)
	{
		
		ss << "\"bbHist\": [";
				
		for (int i = 0; i < dta->bbHistory->size(); i++)
		{
			ss << "[" << (*dta->bbHistory)[i].x << "," << (*dta->bbHistory)[i].y << "," 
			   << (*dta->bbHistory)[i].width << "," << (*dta->bbHistory)[i].height<< "]";
			if (i < dta->bbHistory->size()-1) ss << ", ";
		}
		ss << "]";
		
		ss << ", ";	
	}
	
	if (dta->rollPid)
		ss << "\"RollPid\": [" << dta->rollPid->prm.Kp << "," << dta->rollPid->prm.Ki 
					<< "," << dta->rollPid->prm.Kd << ","<< dta->rollPid->prm.Ku <<"]";
				
	
	if (dta->ALT_EstAlt)
	{
		ss << ", ";
		ss << "\"EstAlt\":" << (*dta->ALT_EstAlt);
	}
	if (dta->ALT_Baro){
		ss << ", ";
		ss << "\"AltBaro\":" << (*dta->ALT_Baro);
	}
	if (dta->ALT_dAlt)
	{
		ss << ", ";
		ss << "\"dAlt\":" << (*dta->ALT_dAlt);
	}
	
	if (dta->ALT_lastAlt)
	{	
		ss << ", ";
		ss << "\"lastAlt\":" << (*dta->ALT_lastAlt);
	}
	if (dta->ThrottG0)
	{	
		ss << ", ";
		ss << "\"ThrottG0\":" << (*dta->ThrottG0);
	}
	//if (dta->ALT_dAlt)
	{	
		ss << ", ";
		ss << "\"ThrottLast\":" << (dta->RCThrottleLast);
	}
	ss << "}";
	
	return ss.str();
}


void sendFrame(DataStruct* dta) {
	
	std::vector<uchar> buffer;
	cv::imencode(".jpg", (*dta->camImage), buffer);
		
	zmq::message_t message(buffer.size());
	memcpy(message.data(), buffer.data(), buffer.size());
	
	std::string telem = "testString!";
	
	if (dta) telem = makeTelemetryString(dta);
		
	zmq::message_t telem_message(telem.size());
	memcpy(telem_message.data(), telem.c_str(), telem.size());
	
	zmqsocket.send(message, zmq::send_flags::sndmore);
	zmqsocket.send(telem_message, zmq::send_flags::dontwait);
	
}


extern "C" {


void init(DataStruct* dta)
{
	std::string port = dta->conf->streamerPort;
	std::string address = "tcp://*:"+port;
	std::cout << "SocketStreamer binding...\n";
	zmqsocket.bind(address);
	std::cout << "Bind address: " << address << "\n";
	
}

void process(DataStruct* dta){
	//std::cout << "sending frame " << "\n";
	sendFrame(dta);
}

} // extern "C"
