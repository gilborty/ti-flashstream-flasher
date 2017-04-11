#include "i2c_interface.h"

I2CInterface::I2CInterface(const std::string& deviceFile, uint8_t slaveAddress)
    : m_deviceFile(deviceFile),
      m_slaveAddress(slaveAddress),
      m_fileDescriptor(0)
{
}

I2CInterface::~I2CInterface()
{
    closeFileDescriptor();
}

I2CInterface::RET_CODE I2CInterface::init()
{
    return openFileDescriptor();
}

I2CInterface::RET_CODE I2CInterface::openFileDescriptor()
{
    if(m_fileDescriptor)
    {
        closeFileDescriptor();
    }

    //Try to open the file descriptor
    m_fileDescriptor = open(m_deviceFile.c_str(), O_RDWR);
    if(m_fileDescriptor < 0)
    {
        std::cout << "i2c error: " << std::strerror(errno) << std::endl;
        return RET_CODE::FAILED_OPEN_FILE_DESCRIPTOR;
    }

    //Init input output control
    if(ioctl(m_fileDescriptor, I2C_SLAVE, m_slaveAddress) < 0)
    {
        std::cout << "i2c error: " << std::strerror(errno) << std::endl;
        return RET_CODE::FAILED_INIT_INPUT_OUTPUT_CONTROL;
    }
    return RET_CODE::SUCCESS;
}

void I2CInterface::closeFileDescriptor()
{
    if(m_fileDescriptor)
    {
        close(m_fileDescriptor);
        m_fileDescriptor = 0;
    }
}

I2CInterface::RET_CODE I2CInterface::setSlaveAddress(const uint8_t slaveAddress)
{
    m_slaveAddress = slaveAddress;
    return openFileDescriptor();
}

uint8_t I2CInterface::getSlaveAddress() const
{
    return m_slaveAddress;
}

I2CInterface::RET_CODE I2CInterface::send(uint8_t registerAddress, uint8_t* txBuffer, int length)
{
    //check for a null buffer pointer
    if(txBuffer == nullptr)
    {
        std::cout << "i2c error: received null transmit buffer." << std::endl;
        return RET_CODE::NULL_TRANSMIT_BUFFER;
    }

    //Check for an invalid length
    if(length < 1)
    {
        std::cout << "i2c error: invalid send buffer length" << std::endl;
        return RET_CODE::INVALID_TRANSMIT_BUFFER_LENGTH;
    }

    //Create the buffer
    //Payload length plus zeroth element for register
    uint8_t buffer[length + 1];
    buffer[0] = registerAddress;

    //Pack the buffer
    for(size_t i = 0; i < length; ++i)
    {
        buffer[i+1] = txBuffer[i];
    }

    //Write the data
    if(write(m_fileDescriptor, buffer, length) != length)
    {
        std::cout << "i2c error: " << std::strerror(errno) << std::endl;
        return RET_CODE::FAILED_I2C_WRITE;
    }

    return RET_CODE::SUCCESS;
}

I2CInterface::RET_CODE I2CInterface::receive(uint8_t registerAddress, uint8_t* rxBuffer, int length)
{
    //Specify the address
    // int retCode = write(registerAddress, , 1);
    // if(retCode != 1)
    // {
    //     std::cout << "i2c error: " << std::strerror(errno) << std::endl;
    //     return RET_CODE::FAILED_I2C_READ;
    // }

    if(read(m_fileDescriptor, rxBuffer, length) != length)
    {
        std::cout << "i2c error: " << std::strerror(errno) << std::endl;
        return RET_CODE::FAILED_I2C_READ;
    }

    return RET_CODE::SUCCESS;
}