#include "filestream_parser.h"


FilestreamParser::FilestreamParser(const std::string& flashstreamFile)
    : m_flashstreamFilename(flashstreamFile)
    , m_i2cInterface()
{
    // Try to open the file for reading  and load it into a ss buffer
    std::ifstream inputStream(m_flashstreamFilename);
    m_flashstreamBuffer << inputStream.rdbuf();
}
FilestreamParser::FilestreamParser(const std::string& flashstreamFile, I2CInterface interfaceIn)
    : m_flashstreamFilename(flashstreamFile)
    , m_i2cInterface(interfaceIn)
{
    // Try to open the file for reading  and load it into a ss buffer
    std::ifstream inputStream(m_flashstreamFilename);
    m_flashstreamBuffer << inputStream.rdbuf();
}
FilestreamParser::FilestreamParser(const std::string& flashstreamFile, const std::string& i2cDevice, int slaveAddress)
    : m_flashstreamFilename(flashstreamFile)
    , m_i2cInterface(i2cDevice, slaveAddress)
{
    // Try to open the file for reading  and load it into a ss buffer
    std::ifstream inputStream(m_flashstreamFilename);
    m_flashstreamBuffer << inputStream.rdbuf();
}

int FilestreamParser::flash()
{
    std::cout << "Starting TI FlashStream flasing process." << std::endl;
    std::cout << "Using data file: " << m_flashstreamFilename << std::endl;

    if(validate())
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
                    handleCompare(token);
                    break;
                }
                case 'W':
                {
                    handleWrite(token);
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

}

bool FilestreamParser::validate()
{
    //Try to parse the file, return false if could not validate
    std::cout << "Validating..." << std::endl;

    std::string token;
    bool isValid = true;
    while(std::getline(m_flashstreamBuffer, token, '\n'))
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

    return isValid;
}

void FilestreamParser::handleComment(const std::string& commentIn)
{
    //; [comment]
    std::cout << commentIn << std::endl;
}

void FilestreamParser::handleCompare(const std::string& compareLine)
{
    //C: i2cAddr RegAddr Byte0 Byte1 Byte2
    auto split = splitString(compareLine, ' ');

    unsigned char slaveAddress = getHexFromString(split[1]);
    unsigned char regAddress = getHexFromString(split[2]);

    auto payload = getPayload(split, 3);

    unsigned char buffer[payload.size()];

    if(slaveAddress != m_i2cInterface.getAddress())
    {
        m_i2cInterface.setAddress(slaveAddress);
    }

    if(m_i2cInterface.receive(regAddress, buffer, payload.size()) < 0)
    {
        throw std::runtime_error("Error writing to device");
    } 

    //Compare
    for(size_t i = 0; i < payload.size(); ++i)
    {
        if(buffer[i] != payload.at(i))
        {
            throw std::runtime_error("Failed compare");
        }
    }
}

void FilestreamParser::handleWrite(const std::string& writeLine)
{
    //W: I2CAddr RegAddr Byte0 Byte1 Byte2…
    auto split = splitString(writeLine, ' ');

    unsigned char slaveAddress = getHexFromString(split[1]);
    unsigned char regAddress = getHexFromString(split[2]);

    auto payload = getPayload(split, 3);

    std::cout << "Writing to device address: " << slaveAddress << " with reg address: " << regAddress << std::endl;

    if(slaveAddress != m_i2cInterface.getAddress())
    {
        m_i2cInterface.setAddress(slaveAddress);
    }

    if(m_i2cInterface.send(regAddress, &payload.front(), payload.size()) < 0)
    {
        throw std::runtime_error("Error writing to device");
    }  
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

unsigned char FilestreamParser::getHexFromString(const std::string& stringIn)
{
    unsigned char value = std::stoul(stringIn, nullptr, 16);
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