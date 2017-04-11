#include "filestream_parser/filestream_parser.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        std::cout << "USAGE: ti-flashstream-flasher [I2C Device] [slave address] [file]" << std::endl;
        return -1;
    }

    std::string i2cDevice = argv[1];
    uint8_t slaveAddress = argv[2];
    std::string flashStreamFile = argv[3];

    //Init i2c interface
    I2CInterface i2cInterface(i2cDevice, slaveAddress);
    
    int returnCode = i2cInterface.init();
    if(returnCode != I2CInterface::RET_CODE::SUCCESS)
    {
        std::cout << "Failed i2c init with return code: " << returnCode << std::endl;
        return -1;
    }


    //Init filestream parser





    return 0;
}

