#include "../include/DataStruct.hpp"
#include <iostream>

// This is sample code for custom purpose module

int someCustomVariable = 0;

extern "C" {


void init(DataStruct* dta)
{
	// some code to init, if needed
	std::cout << "Custom module init called!\n"; 
}

void process(DataStruct* dta){
	std::cout << "Custom module process called\n";
}

} // extern "C"
