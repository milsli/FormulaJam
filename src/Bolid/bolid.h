#ifndef BOLID_H
#define BOLID_H

#include<jam_module.h>
#include <random>

class  Bolid : public JAM::Module
{
public:
    int Setup( int argc, char * argv[] );
    int Work( void );
    int Cleanup( void );

private:
    std::random_device rd;
    std::string driver = "F1";
    int start = 100;
    int speed = 100;
};

JAM_MODULE_ENTRY( Bolid )

#endif // BOLID_H
