#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

class FilestreamParser
{
    public:
        FilestreamParser(const std::string& flashstreamFile);

        int flash();

    private:

        std::string m_flashstreamFilename;
        std::stringstream m_flashstreamBuffer;

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