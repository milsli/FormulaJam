#ifndef REFEREE_H
#define REFEREE_H

#include<jam_module.h>
#include <map>

struct Results;

class  Referee : public JAM::Module
{
public:
    int Setup( int argc, char * argv[] );
    int Work( void );
    int Cleanup( void );

private:
    void showRaceResults(Results *results);
    void showStartResults(Results *results);
    int newRace(Results *results);
    void setRanking(Results *results);

private:
    int number_of_races;
    int number_of_laps;
    int lap_number;
    int current_race;
    std::map<std::string, int> ranking;
};

JAM_MODULE_ENTRY( Referee )

#endif // REFEREE_H
