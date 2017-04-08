#include "i2c_interface.h"

const std::string I2CInterface::m_defaultDeviceFile = "/dev/i2c-1";

I2CInterface::I2CInterface()
    : m_deviceFile(m_defaultDeviceFile),
      m_fd(0),
      m_errorFlag(false),
      m_slaveAddress(-1)
{
    if(openFD() < 0)
    {
        //throw std::runtime_error("I2C ERROR: Could not open file descriptor");
    }
}

I2CInterface::I2CInterface(unsigned char address)
    : m_deviceFile(m_defaultDeviceFile),
      m_fd(0),
      m_errorFlag(false),
      m_slaveAddress(address)
{
    if(openFD() < 0)
    {
        throw std::runtime_error("I2C ERROR: Could not open file descriptor");
    }
}

I2CInterface::I2CInterface(std::string deviceFile, int address)
    : m_deviceFile(deviceFile),
      m_fd(0),
      m_errorFlag(false),
      m_slaveAddress(address)
{
    if(openFD() < 0)
    {
        throw std::runtime_error("I2C ERROR: Could not open file descriptor");
    }
}

I2CInterface::~I2CInterface()
{
    closeFD();
}


int I2CInterface::openFD()
{
    m_errorFlag = false;

    if(m_fd)
    {
        closeFD();
        m_fd = 0;
    }

    //Bit shift slave address
    m_slaveAddress = (m_slaveAddress >> 1);

    if(m_slaveAddress == -1)
    {
        return errorMessage("ERROR: Slave address is not set");
    }

    if( ( m_fd = open( m_deviceFile.c_str(), O_RDWR ) ) < 0 )
    {
        return errorMessage("ERROR opening: " + m_deviceFile);
    }

    if( ioctl(m_fd, I2C_SLAVE, m_slaveAddress) < 0)
    {
        return errorMessage("ERROR: input/output control ");
    }

    return 0;
}

void I2CInterface::closeFD()
{
    if(m_fd)
    {
        close(m_fd);
        m_fd = 0;
    }
}

int I2CInterface::errorMessage(const std::string& message)
{
    m_errorFlag = true;
    
    std::cout << message << std::endl;

    closeFD();
    return -1;
}

int I2CInterface::setAddress(unsigned char address)
{
    m_slaveAddress = address;
    return openFD();
}

unsigned char I2CInterface::getAddress() const 
{
    return m_slaveAddress;
}

int I2CInterface::receive(unsigned char registerAddress, unsigned char* rxBuffer, int length)
{
    if(rxBuffer == 0)
    {
        return errorMessage("Recieve function receieved a null rx buffer pointer");
    }

    if(length < 1)
    {
        return errorMessage("Recieve function receieved an invalid buffer length");
    }

    if(!m_fd)
    {
        if(openFD() == -1)
        {
            return errorMessage("ERROR: Could not open fd");
        }
    }

    m_errorFlag = false;

    if( write(m_fd, &registerAddress, 1) != 1)
    {
        return errorMessage("ERROR: i2c write error");
    }

    if( read(m_fd, rxBuffer, length) != length )
    {
        return errorMessage("ERROR: i2c read error!");
    }
    return 1;
}

int I2CInterface::send(unsigned char registerAddress, unsigned char* txBuffer, int length)
{
	unsigned char data[length+1];
	data[0] = registerAddress;
	
	for ( size_t i = 0; i < length ; ++i ) 
    {
		data[i+1] = txBuffer[i];
	}

	if (txBuffer == 0)
    {
        return errorMessage("Null tx buffer pointer!");
    }
	if (length < 1)
    {
        return errorMessage("Send method received an invalid buffer length.");
    }
		
    if(!m_fd)
    {
        if(openFD() == -1)
        {
            return errorMessage("ERROR: Could not open fd");
        }
    }

	m_errorFlag = false;	

    //TODO: Get the actual ioctl error code reported here
    //http://stackoverflow.com/questions/14968238/ioctl-call-and-checking-return-value
    for(size_t i = 0; i < length + 1; ++i)
    {
        std::cout <<"data[" << i << "]" << "\t" << (int)data[i] << std::endl;
    }
	if(write(m_fd, data, length+1) != length+1)
    {
        printf("ioctl failed and returned errno %s \n",strerror(errno));
        return errorMessage("i2c write error!");
    }

	return 1;
}