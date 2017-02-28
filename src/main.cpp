//I2C includes 
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

//General includes
#include <chrono>
#include <errno.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <thread>

void wait(int milliseconds)
{
    //Sleeps this thread for the specified milliseconds
    std::cout << "Waiting for " << milliseconds << " milliseconds." << std::endl; 
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main(int argc, const char* argv[])
{
    std::cout << "Starting ti-flashstream-flasher" << std::endl;

    //Get command line argument to file data file
    std::ifstream input_image_file(argv[1]);

    //Open the i2c device
    int i2c_handle;
    char* i2c_path = (char*)"/dev/i2c-1";

    std::cout << "Trying to open: " << i2c_path << std::endl;
    if((i2c_handle = open(i2c_path, O_RDWR)) < 0)
    {
        std::cout << "Failed to open the i2c bus!" << std::endl;
        std::cout << "Error: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Opened: " << i2c_path << std::endl; 

    
    //Iterate through each line in the input file
    for(std::string line; getline(input_image_file, line); )
    {
        std::cout << line << std::endl;
    } 

    return 0;
}