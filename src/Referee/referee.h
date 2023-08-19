#ifndef REFEREE_H
#define REFEREE_H

#include<jam_module.h>

struct Results;

class  Referee : public JAM::Module
{
public:
    int Setup( int argc, char * argv[] );
    int Work( void );
    int Cleanup( void );

private:
    void showResults(Results *results);

private:
    int number_of_laps;
    int lap_number;
};

JAM_MODULE_ENTRY( Referee )

#endif // REFEREE_H
