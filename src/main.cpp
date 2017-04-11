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

    //Get the i2c device file (i.e.: /dev/i2c-2)
    std::string i2cDevice = argv[1];

    //slave address we will start writing to (i.e.: 55)
    std::string addr = argv[2];
    uint8_t slaveAddress = std::stoul(addr, nullptr, 16);

    //The path to the ti flashstream file
    std::string flashStreamFile = argv[3];

    //construct a parser and flasher
    FilestreamParser parser(flashStreamFile, i2cDevice, slaveAddress);
    
    //Try to init
    I2CInterface::RET_CODE retCode = parser.init();
    if(retCode != I2CInterface::RET_CODE::SUCCESS)
    {
        std::cout << "Failed init with return code: " << static_cast<int>(retCode) << std::endl;
        return -1;
    }

    //Try to flash
    retCode = parser.flash();
    if(retCode != I2CInterface::RET_CODE::SUCCESS)
    {
        std::cout << "Failed flashing with return code: " << static_cast<int>(retCode) << std::endl;
        return -1;
    }

    std::cout << "Flashing successful." << std::endl;
    return 0;
}

