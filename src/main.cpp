#include "filestream_parser/filestream_parser.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        std::cout << "USAGE: ti-flashstream-flasher [I2C Device] [Slave Address] [file]" << std::endl;
        return -1;
    }

    std::string i2cDevice = argv[1];

    std::string slaveAddress = argv[2];
    unsigned char shiftedAddress = (std::stoul(slaveAddress, nullptr, 16) >> 1);

    std::string flashStreamFile = argv[3];

    std::cout << "opening i2c device: " << i2cDevice << " with slave address: " << (int)shiftedAddress << std::endl;
    
    I2CInterface i2cInterface(i2cDevice, shiftedAddress);
    FilestreamParser parser(flashStreamFile, i2cInterface);

    parser.flash();

    return 0;
}

