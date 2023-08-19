#ifndef RESULTS_H
#define RESULTS_H

#include <jam_packet.h>
#include <map>

struct Results: public JAM::Packet
{
    Results( void ) : JAM::Packet( "Results", sizeof( Results ) )
    {
        drivers_count = 0;
    }

    static const unsigned int max_drivers = 30;
    std::map<std::string, int>          result; // w razie czego dać tu wskaźnik
    int drivers_count;
};


#endif // RESULTS_H
