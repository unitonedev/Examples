// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Now PIDController.cpp in utilsPrj !!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifndef PIDCONTROLLER_HPP
#define PIDCONTROLLER_HPP

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// PID Controller parameters
struct PIDParameters {
    double Kp;   // Proportional gain
    double Ki;   // Integral gain
    double Kd;   // Derivative gain
	double Ku;
	PIDParameters() : Kp(0.7), Ki(0.05), Kd(0.3), Ku(1.0){}
	PIDParameters(double KP, double KI, double KD, double KU) : Kp(KP*KU), Ki(KI*KU), Kd(KD*KU), Ku(KU){} 
};

// PID Controller class
class PIDController {
public:
    PIDController(PIDParameters parameters);
    void reset();
    double compute(double setpoint, double pv, double dt);

//private:
    //double Kp, Ki, Kd;
	PIDParameters prm;
    double lastError;
    double integral;
};

PIDParameters pidParamsDefault();

// Autotune function using Ziegler-Nichols method
PIDParameters autotune(double setpoint, double (*process_func)(double), double max_output);

#endif // PIDCONTROLLER_HPP
