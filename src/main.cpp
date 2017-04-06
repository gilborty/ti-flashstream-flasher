//I2C includes 


//General includes
#include <chrono>
#include <errno.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <thread>


#include "i2c/i2c_interface.h"

void wait(int milliseconds)
{
    //Sleeps this thread for the specified milliseconds
    std::cout << "Waiting for " << milliseconds << " milliseconds." << std::endl; 
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main(int argc, const char* argv[])
{
    I2CInterface interface(3);
    std::cout << "Starting ti-flashstream-flasher" << std::endl;

    return 0;
}

