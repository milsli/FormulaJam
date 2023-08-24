#include "referee.h"
#include "results.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int Referee::Setup(int argc, char *argv[])
{
    number_of_races = 16;
    number_of_laps = 100;
    current_race = 1;
    lap_number = 0;
    for(int i = 0; i < argc; ++i)
    {
        if(std::strcmp(argv[i], "-round") == 0) {
            number_of_laps = std::atoi(argv[i + 1]);
        }
        if(std::strcmp(argv[i], "-season") == 0) {
            number_of_races = std::atoi(argv[i + 1]);
        }
    }

    Results * res = JAM::RetrievePacket( GetPacketPool(), res );

    for(const std::pair<std::string, int> result : res->result)
    {
        ranking[result.first] = 0;
    }

    showStartResults(res);
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
        showRaceResults(res);
        std::this_thread::sleep_for(600ms);
        ++lap_number;

        if(lap_number > number_of_laps)
            return newRace(res);
    }


    return JAM::Module::Work();
}

int Referee::Cleanup()
{
    return JAM::Module::Cleanup();
}

void Referee::showRaceResults(Results *results)
{
    if(lap_number == 0)
        return;

    std::multimap<int, std::string> sorted_multimap;
    for(auto it = results->result.begin(); it != results->result.end(); ++it )
    {
        sorted_multimap.insert(std::pair<int, std::string>(it->second, it->first));
    }

    if(lap_number > 0)
        std::cout << "\n\n WYNIKI WYSCIGU PO " << lap_number << "/" << number_of_laps << " OKRAZENIU:\n";

    int count = 1;

    for(auto it = sorted_multimap.rbegin(); it != sorted_multimap.rend(); ++it)
    {
        if(-1 == it->first)
            std::cout << " -- " << it->second << "   CRASHED\n";
        else
        {
            std::cout << count++ << ": " << it->second << "    dystans  " << it->first << "\n";
        }
    }
}

void Referee::showStartResults(Results *results)
{
    if(lap_number > 0)
        return;

    std::multimap<int, std::string> sorted_multimap;
    for(auto it = results->result.begin(); it != results->result.end(); ++it )
    {
        sorted_multimap.insert(std::pair<int, std::string>(it->second, it->first));
    }

    if(lap_number == 0)
        std::cout << "\n\n WYNIKI WYSCIGU PO STARCIE:\n";

    int count = 1;

    for(auto it = sorted_multimap.rbegin(); it != sorted_multimap.rend(); ++it)
    {
        std::cout << count++ << ": " << it->second << "    dystans  " << it->first << "\n";
    }
}

int Referee::newRace(Results *results)
{
    setRanking(results);
    results->result.clear();

    if(current_race == number_of_races)
        return 1;

    lap_number = 0;
    std::cout << "\n\nWYSCIG  " << ++current_race << "/" << number_of_races << "\n";
    showStartResults(results);


    return JAM::Module::Work();
}

void Referee::setRanking(Results *results)
{
    std::multimap<int, std::string> sorted_multimap;
    for(auto it = results->result.begin(); it != results->result.end(); ++it )
    {
        sorted_multimap.insert(std::pair<int, std::string>(it->second, it->first));
    }

    int count = 1;

    for(auto it = sorted_multimap.rbegin(); it != sorted_multimap.rend(); ++it)
    {
        if(-1 == it->first)
            ; // std::cout << " -- " << it->second << "   CRASHED\n";
        else
        {
            //std::cout << count << ": " << it->second << "    dystans  " << it->first << "\n";

            int scores = 0;
            switch (count) {
            case 1:
                scores = 25;
                break;
            case 2:
                scores = 20;
                break;
            case 3:
                scores = 15;
                break;
            case 4:
                scores = 10;
                break;
            case 5:
                scores = 8;
                break;
            case 6:
                scores = 6;
                break;
            case 7:
                scores = 5;
                break;
            case 8:
                scores = 3;
                break;
            case 9:
                scores = 2;
                break;
            case 10:
                scores = 1;
                break;
            default:
                scores = 0;
                break;
            }

            ranking[it->second] += scores;
            ++count;
        }
    }

    int rank = 1;
    std::cout << "\nRANKING PO WYSCIGU\n";

    std::multimap<int, std::string> sorted_ranking_multimap;

    for(auto it = ranking.begin(); it != ranking.end(); ++it )
    {
        sorted_ranking_multimap.insert(std::pair<int, std::string>(it->second, it->first));
    }

    for(auto it = sorted_ranking_multimap.rbegin(); it != sorted_ranking_multimap.rend(); ++it)
    {
        std::cout << " " << rank++ << ". " << it->second << "       " << it->first << "\n";
    }
}
