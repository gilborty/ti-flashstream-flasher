#include "filestream_parser/filestream_parser.h"

void wait(int milliseconds)
{
    //Sleeps this thread for the specified milliseconds
    std::cout << "Waiting for " << milliseconds << " milliseconds." << std::endl; 
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main(int argc, const char* argv[])
{
    FilestreamParser parser(argv[1]);
    parser.flash();

    return 0;
}

