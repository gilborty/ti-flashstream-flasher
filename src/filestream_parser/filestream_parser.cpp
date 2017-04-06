#include "filestream_parser.h"


FilestreamParser::FilestreamParser(const std::string& flashstreamFile)
    : m_flashstreamFilename(flashstreamFile)
{
    // Try to open the file for reading  and load it into a ss buffer
    std::ifstream inputStream(m_flashstreamFilename);
    m_flashstreamBuffer << inputStream.rdbuf();
}

int FilestreamParser::flash()
{
    std::cout << "Starting TI FlashStream flasing process." << std::endl;

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
                std::cout << "Error parsing file." << std::endl;
                break;
            }
        }
    }
}

void FilestreamParser::handleComment(const std::string& commentIn)
{
    //; [comment]
    std::cout << commentIn << std::endl;
}

void FilestreamParser::handleCompare(const std::string& commentIn)
{
    //C: i2cAddr RegAddr Byte0 Byte1 Byte2
    std::cout << "Comparing" << std::endl;
}

void FilestreamParser::handleWrite(const std::string& commentIn)
{
    //W: I2CAddr RegAddr Byte0 Byte1 Byte2…
    std::cout << "Writing" << std::endl;
}

void FilestreamParser::handleWait(const std::string& commentIn)
{
    //X: [ms for wait]
    auto split = splitString(commentIn, ' ');
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