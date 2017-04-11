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
        FilestreamParser(const std::string& flashstreamFile, const std::string& deviceFile, const uint8_t slaveAddress);

        I2CInterface::RET_CODE init();
        I2CInterface::RET_CODE flash();

    private:
        std::string m_flashstreamFilename;
        std::stringstream m_flashstreamBuffer;
        I2CInterface m_i2cInterface;

        uint8_t getHexFromString(const std::string& stringIn);
        std::vector<uint8_t> getPayload(std::vector<std::string>& lineIn, int pruneLength);

        void handleComment(const std::string& commentIn);
        void handleWait(const std::string& waitLine);

        I2CInterface::RET_CODE handleCompare(const std::string& compareLine);
        I2CInterface::RET_CODE handleWrite(const std::string& writeLine);

        std::vector<std::string> splitString(const std::string& s, char delim);
        bool validate();
        void wait(int milliseconds);

        template<typename Out>
        void split(const std::string &s, char delim, Out result) {
            std::stringstream ss;
            ss.str(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                *(result++) = item;
            }
        }
};