#include "bolid.h"
#include <stdlib.h>
#include "results.h"

int Bolid::Setup(int argc, char *argv[])
{
    for(int i = 0; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "-driver") == 0) {
            driver = argv[i + 1];
        }
        if(std::strcmp(argv[i], "-start") == 0) {
            start = atoi(argv[i + 1]);
        }
        if(std::strcmp(argv[i], "-speed") == 0) {
            speed = atoi(argv[i + 1]);
        }
    }

    Results * res = JAM::RetrievePacket( GetPacketPool(), res );
    ++res->drivers_count;

    // start
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(2, 9);

    unsigned int start_distance = static_cast<unsigned int>(((dist(mt)  + 100) * start) / 100);
    res->result[driver] =  start_distance;
    //

    return JAM::Module::Setup( argc, argv );
}

int Bolid::Work()
{
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(0, 15);

    Results * res = JAM::RetrievePacket( GetPacketPool(), res );
    unsigned int distance = static_cast<unsigned int>(((dist(mt)  + 100) * speed) / 100);

    if(-1 != res->result[driver])
        res->result[driver] += distance;

    return JAM::Module::Work();
}

int Bolid::Cleanup()
{
    return JAM::Module::Cleanup();
}
