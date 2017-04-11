#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cstring>
#include <cerrno>


class I2CInterface
{
    public:

        enum class RET_CODE : int
        {
            SUCCESS = 0,
            FAILED_OPEN_FILE_DESCRIPTOR = 1,
            FAILED_INIT_INPUT_OUTPUT_CONTROL = 2,
            NULL_TRANSMIT_BUFFER = 3,
            INVALID_TRANSMIT_BUFFER_LENGTH = 4,
            FAILED_I2C_WRITE = 5,
            FAILED_I2C_READ = 6,
            FAILED_VALIDATE = 7
        };

        I2CInterface(const std::string& deviceFile, uint8_t slaveAddress);
        ~I2CInterface();

        I2CInterface::RET_CODE init();
        I2CInterface::RET_CODE  openFileDescriptor();
        void closeFileDescriptor();

        I2CInterface::RET_CODE setSlaveAddress(uint8_t slaveAddress);
        uint8_t getSlaveAddress() const;

        I2CInterface::RET_CODE receive(uint8_t registerAddress, uint8_t* rxBuffer, int length);
        I2CInterface::RET_CODE send(uint8_t registerAddress, uint8_t* txBuffer, int length);

    private:
        std::string m_deviceFile;
        uint8_t m_slaveAddress;
        int m_fileDescriptor;
};