#include "referee.h"
#include "results.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int Referee::Setup(int argc, char *argv[])
{
    number_of_laps = 100;
    lap_number = 0;
    for(int i = 0; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "-round") == 0) {
            number_of_laps = std::atoi(argv[i + 1]);
        }
    }

    Results * res = JAM::RetrievePacket( GetPacketPool(), res );
    showResults(res);
    lap_number = 1;
    return JAM::Module::Setup( argc, argv );
}

int Referee::Work()
{
    Results * res = JAM::RetrievePacket( GetPacketPool(), res );
    bool next_lap = true;
    for(const std::pair<std::string, int> result : res->result)
    {
        if(-1 != result.second && (lap_number * 2000) >= result.second)   // 2000 m to długość okrążenia
        {
            next_lap = false;
            break;
        }
    }

    if(next_lap == true)
    {
        showResults(res);
        std::this_thread::sleep_for(300ms);
        ++lap_number;

        if(lap_number > number_of_laps)
            return 1;
    }

    return JAM::Module::Work();
}

int Referee::Cleanup()
{
    return JAM::Module::Cleanup();
}

void Referee::showResults(Results *results)
{
    std::multimap<int, std::string> sorted_multimap;
    for(auto it = results->result.begin(); it != results->result.end(); ++it )
    {
        sorted_multimap.insert(std::pair<int, std::string>(it->second, it->first));
    }
    if(lap_number > 0)
        std::cout << "\n\n WYNIKI WYSCIGU PO " << lap_number << " OKRAZENIU:\n";
    else
        std::cout << "\n\n WYNIKI WYSCIGU PO STARCIE:\n";
    for(auto it = sorted_multimap.rbegin(); it != sorted_multimap.rend(); ++it)
    {
        if(-1 == it->first)
            std::cout << "Kierowca: " << it->second << "   CRASHED\n";
        else
            std::cout << "Kierowca: " << it->second << "    dystans  " << it->first << "\n";
    }
}
