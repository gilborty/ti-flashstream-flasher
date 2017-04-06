#include "filestream_parser/filestream_parser.h"

int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        std::cout << "USAGE: ti-flashstream-flasher [I2C Device] [Slave Address] [file]" << std::endl;
        return -1;
    }

    std::string i2cDevice = argv[1];
    int slaveAddress = std::stoi(argv[2]);
    std::string flashStreamFile = argv[3];

    std::cout << "opening i2c device: " << i2cDevice << " with slave address: " << slaveAddress << std::endl;

    //I2CInterface i2cInterface(i2cDevice, slaveAddress);
    FilestreamParser parser(flashStreamFile);

    parser.flash();

    return 0;
}

