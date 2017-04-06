#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "i2c_interface.h"
class FilestreamParser
{
    public:
        FilestreamParser(const std::string& flashstreamFile);
        FilestreamParser(const std::string& flashstreamFile, I2CInterface interfaceIn);
        FilestreamParser(const std::string& flashstreamFile, const std::string& i2cDevice, int slaveAddress);

        int flash();
    private:

        std::string m_flashstreamFilename;
        std::stringstream m_flashstreamBuffer;
        I2CInterface m_i2cInterface;

        void handleComment(const std::string& commentIn);
        void handleCompare(const std::string& compareLine);
        void handleWait(const std::string& waitLine);
        void handleWrite(const std::string& writeLine);

        template<typename Out>
        void split(const std::string &s, char delim, Out result) {
            std::stringstream ss;
            ss.str(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                *(result++) = item;
            }
        }
        std::vector<std::string> splitString(const std::string& s, char delim);
        void wait(int milliseconds);
   
};