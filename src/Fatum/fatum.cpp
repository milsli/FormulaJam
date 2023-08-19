#include "fatum.h"
#include "results.h"

int Fatum::Setup(int argc, char *argv[])
{
    return JAM::Module::Setup( argc, argv );
}

int Fatum::Work()
{

    Results * res = JAM::RetrievePacket( GetPacketPool(), res );

    auto it = res->result.begin();
    if (it->second < 1500)
        return JAM::Module::Work();

    std::mt19937 mt(rd());
    std::uniform_int_distribution<> crash_dist(1, 1000);
    std::uniform_int_distribution<> distance_dist(-1, 1500);
    std::uniform_int_distribution<> driver_dist(0, res->drivers_count - 1);


    int crash_probability = crash_dist(mt);
    if(crash_probability < 100)
    {
        unsigned int lost_distance = distance_dist(mt);
        int lost_driver = driver_dist(mt);

        auto start_it = res->result.begin();
        for(int i = 0; i < lost_driver; ++i)
            ++start_it;

        if(lost_distance < 50) {
            start_it->second = -1;
            std::cout << " >>  Car breakdown: " << start_it->first << "  <<\n";
        }
        else {
            if(start_it->second != -1) {
                start_it->second -= lost_distance;
                std::cout << " >>  Car accident: " << start_it->first << "   lost distance: " << lost_distance << "  <<\n";
            }
        }
    }

    return JAM::Module::Work();
}

int Fatum::Cleanup()
{
    return JAM::Module::Cleanup();
}
