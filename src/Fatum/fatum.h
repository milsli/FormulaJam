#ifndef FATUM_H
#define FATUM_H

#include<jam_module.h>
#include <random>

class  Fatum : public JAM::Module
{
public:
    int Setup( int argc, char * argv[] );
    int Work( void );
    int Cleanup( void );
private:
    std::random_device rd;
};

JAM_MODULE_ENTRY( Fatum )

#endif // FATUM_H
