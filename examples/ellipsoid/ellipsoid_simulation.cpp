#include "../header.h"

using namespace std;

int main(){

    // Construct settings: int N_PARTICLES_,double LAMBDA_,double SW_C_,double D0_,int SEED_
    Settings settings(100000,0.001,0.002,0.004,888);

    // Construct body
    Ellipsoid ellipsoid(3.0);
    ellipsoid.make_mesh(32);
 
    // Construct sampler
    Sampler sampler(ellipsoid.get_num_surface_states());

    double R0 = ellipsoid.get_max_extend()*1.1;
    // Construct source
    Uniform_source uniform_source(R0,settings);

    // Construct simulation
     Simulation<Ellipsoid,Uniform_source,Sampler> sim(ellipsoid,uniform_source,sampler,settings,R0);

    sim.setup(".");
    sim.run();

    return 0;
}