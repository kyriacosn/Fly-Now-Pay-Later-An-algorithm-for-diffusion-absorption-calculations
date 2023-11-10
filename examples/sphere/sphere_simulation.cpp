#include "../header.h"

using namespace std;

int main(){

    // Construct settings: int N_PARTICLES_,double LAMBDA_,double SW_C_,double D0_,int SEED_
    Settings settings(100000,0.001,0.002,0.004,888);

    // Construct body
    Sphere sphere;
    sphere.make_mesh(32);
 
    // Construct sampler
    Sampler sampler(sphere.get_num_surface_states());

    double R0 = 2.0;
    // Construct source
    Point_source point_source(0,0,2.0);

    // Construct simulation
     Simulation<Sphere,Point_source,Sampler> sim(sphere,point_source,sampler,settings,R0);

    sim.setup(".");
    sim.run();

    return 0;
}