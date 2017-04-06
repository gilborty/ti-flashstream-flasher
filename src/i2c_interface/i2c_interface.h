#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <iostream>
#include <stdexcept>
#include <string>


class I2CInterface
{
    public:

        I2CInterface();
        I2CInterface(int address);
        I2CInterface(std::string deviceFile, int address);

        ~I2CInterface();

        int setAddress(int address);
        int getAddress() const;

        int receive(unsigned char registerAddress, unsigned char* rxBuffer, int length);
        int send(unsigned char registerAddress, unsigned char* txBuffer, int length);

    private:
        std::string m_deviceFile;
        bool m_errorFlag;
        int m_fd;
        int m_slaveAddress;
        
        static const std::string m_defaultDeviceFile;
        
        int errorMessage(const std::string& message);
        int openFD();
        void closeFD();
};