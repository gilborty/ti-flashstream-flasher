#include "filestream_parser.h"

FilestreamParser::FilestreamParser(const std::string& flashstreamFile, const std::string& deviceFile, const uint8_t slaveAddress)
    : m_flashstreamFilename(flashstreamFile)
    , m_i2cInterface(deviceFile, slaveAddress)
{
    // Try to open the file for reading  and load it into a ss buffer
    std::ifstream inputStream(m_flashstreamFilename);
    m_flashstreamBuffer << inputStream.rdbuf();
}

I2CInterface::RET_CODE FilestreamParser::init()
{
    //Setup i2c
    I2CInterface::RET_CODE retCode = m_i2cInterface.init();
    if(retCode != I2CInterface::RET_CODE::SUCCESS)
    {
        std::cout << "Failed i2c init with return code: " << static_cast<int>(retCode) << std::endl;
        return retCode;
    }

    return retCode;
}

I2CInterface::RET_CODE FilestreamParser::flash()
{
    std::cout << "Starting TI FlashStream flasing process." << std::endl;
    std::cout << "Using data file: " << m_flashstreamFilename << std::endl;

    if( validate() )
    {
        std::string token;
        while(std::getline(m_flashstreamBuffer, token, '\n'))
        {
            //Get the key
            char key = token[0];
            
            switch(key)
            {
                case ';':
                {
                    handleComment(token);
                    break;
                }
                case 'C':
                {
                    I2CInterface::RET_CODE retCode = handleCompare(token);
                    if(retCode != I2CInterface::RET_CODE::SUCCESS)
                    {
                        std::cout << "Failed write because of error code: " << static_cast<int>(retCode) << std::endl;
                        return retCode;
                    }
                    break;
                }
                case 'W':
                {
                    I2CInterface::RET_CODE retCode = handleWrite(token);
                    if(retCode != I2CInterface::RET_CODE::SUCCESS)
                    {
                        std::cout << "Failed write because of error code: " << static_cast<int>(retCode) << std::endl;
                        return retCode;
                    }
                    break;
                }
                case 'X':
                {
                    handleWait(token);
                    break;
                }
                default:
                {
                    std::cout << "Error parsing file. Bad line: " << token << std::endl;
                    break;
                }
            }
        }
    }
    else
    {
        std::cout << "Bad input file." << std::endl;
        return I2CInterface::RET_CODE::FAILED_VALIDATE;
    }

    return I2CInterface::RET_CODE::SUCCESS;

}

bool FilestreamParser::validate()
{
    //Try to parse the file, return false if could not validate
    std::cout << "Validating flash stream file..." << std::endl;

    std::stringstream tmpBuffer;
    tmpBuffer << m_flashstreamBuffer.str();

    std::string token;
    bool isValid = true;
    while(std::getline(tmpBuffer, token, '\n'))
    {
        //Get the key
        char key = token[0];
        
        switch(key)
        {
            case ';':
            {
                break;
            }
            case 'C':
            {
                break;
            }
            case 'W':
            {
                break;
            }
            case 'X':
            {
                break;
            }
            default:
            {
                std::cout << "Error parsing file. Bad line: " << token << std::endl;
                isValid = false; 
                break;
            }
        }
    }
    std::cout << "File is valid" << std::endl;

    return isValid;
}

void FilestreamParser::handleComment(const std::string& commentIn)
{
    //; [comment]
    std::cout << commentIn << std::endl;
}

I2CInterface::RET_CODE FilestreamParser::handleCompare(const std::string& compareLine)
{
    //C: i2cAddr RegAddr Byte0 Byte1 Byte2
    auto split = splitString(compareLine, ' ');

    //get the slave address
    uint8_t slaveAddress = (getHexFromString(split[1]));

    //Bit shift left by one
    slaveAddress = (slaveAddress >> 1);
    
    uint8_t regAddress = getHexFromString(split[2]);
    auto payload = getPayload(split, 3);

    uint8_t readBuffer[payload.size()];

    if(slaveAddress != m_i2cInterface.getSlaveAddress())
    {
        I2CInterface::RET_CODE retCode = m_i2cInterface.setSlaveAddress(slaveAddress);
        if(retCode != I2CInterface::RET_CODE::SUCCESS)
        {
            std::cout << "Failed to set address in a handle compare. Returned with error code: " << static_cast<int>(retCode) << std::endl;
            return retCode;
        }
    }

    //Read the bytes
    I2CInterface::RET_CODE retCode = m_i2cInterface.readFromRegister(regAddress, readBuffer, payload.size());
    if(retCode != I2CInterface::RET_CODE::SUCCESS)
    {
        std::cout << "Failed handle write because of error: " << static_cast<int>(retCode) << std::endl;
        return retCode;
    }

    //Compare
    for(size_t i = 1; i < payload.size(); ++i)
    {
        std::cout << "Comparing: " << static_cast<int>(readBuffer[i]) << "\t" << static_cast<int>(payload.at(i)) << std::endl;
        if(readBuffer[i] != payload.at(i))
        {
           std::cout << "Failed compare: {" << static_cast<int>(readBuffer[i]) << " != " << static_cast<int>(payload.at(i)) << "}" << std::endl;
           std::cout << "Continuing..." << std::endl;
        }
    }

    return I2CInterface::RET_CODE::SUCCESS;
}

I2CInterface::RET_CODE FilestreamParser::handleWrite(const std::string& writeLine)
{
    //W: I2CAddr RegAddr Byte0 Byte1 Byte2…
    auto split = splitString(writeLine, ' ');

    uint8_t slaveAddress = (getHexFromString(split[1]));

    //Bit shift left by one
    slaveAddress = (slaveAddress >> 1);

    uint8_t regAddress = getHexFromString(split[2]);
    auto payload = getPayload(split, 3);

    std::cout << "Writing to device address: " << (int)slaveAddress << " with reg address: " << (int)regAddress << std::endl;

    if(slaveAddress != m_i2cInterface.getSlaveAddress())
    {
        I2CInterface::RET_CODE retCode = m_i2cInterface.setSlaveAddress(slaveAddress);
        if(retCode != I2CInterface::RET_CODE::SUCCESS)
        {
            std::cout << "Failed to set address in a handle write. Returned with error code: " << static_cast<int>(retCode) << std::endl;
            return retCode;
        }
    }

    //Iterate throught the payload
    //See Section 1.3: http://www.ti.com/lit/an/slua541a/slua541a.pdf
    for(auto& data : payload)
    {       
        I2CInterface::RET_CODE retCode = m_i2cInterface.sendByte(regAddress, data);
        if(retCode != I2CInterface::RET_CODE::SUCCESS)
        {
            std::cout << "Failed handle write because of error: " << static_cast<int>(retCode) << std::endl;
            return retCode;
        }
        ++regAddress;
    }

    return I2CInterface::RET_CODE::SUCCESS;
}

void FilestreamParser::handleWait(const std::string& waitLine)
{
    //X: [ms for wait]
    auto split = splitString(waitLine, ' ');
    auto waitTime = split[1];

    std::cout << "Waiting for: " << waitTime << " ms" << std::endl;
    wait(std::stoi(waitTime));
}

std::vector<std::string> FilestreamParser::splitString(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

void FilestreamParser::wait(int milliseconds)
{
    //Sleeps this thread for the specified milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

uint8_t FilestreamParser::getHexFromString(const std::string& stringIn)
{
    uint8_t value = std::stoul(stringIn, nullptr, 16);
    return value;
}

std::vector<unsigned char> FilestreamParser::getPayload(std::vector<std::string>& lineIn, int pruneLength)
{
    lineIn.erase(lineIn.begin(), lineIn.begin() + pruneLength);

    std::vector<unsigned char> payload;
    for(auto& elem : lineIn)
    {
        payload.push_back(getHexFromString(elem));
    }
    
    return payload;
}