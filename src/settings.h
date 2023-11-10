#include "../include/xoroshiro128plus.h"

class Settings{
    public:
    int N_PARTICLES;
    double D0;
    double SW_C; 
    double LAMBDA;
    int SEED;
    xoroshiro128plus GEN;
    
    Settings(int N_PARTICLES_,double LAMBDA_,double SW_C_,double D0_,int SEED_):
        N_PARTICLES (N_PARTICLES_),
        LAMBDA      (LAMBDA_),
        SW_C        (SW_C_), 
        D0          (D0_),
        SEED        (SEED_),
        GEN(SEED){}


    std::string details(){

        return  "\t\"N_particles\" : "  + std::to_string(N_PARTICLES) +
                ",\n\t\"D0\" : "        + std::to_string(D0) +
                ",\n\t\"SW_C\" : "      + std::to_string(SW_C) +
                ",\n\t\"LAMBDA\" : "    + std::to_string(LAMBDA) +
                ",\n\t\"SEED\" : "      + std::to_string(SEED);
    }
};
