#include <math.h>
#include <random>
#define TWO_PI 6.283185307179586

class Uniform_source{

    private:

	std::uniform_real_distribution<double> m1to1,zto2pi;
    double const R0;
    Settings & settings;
    
    public:

    Uniform_source(double R0,Settings & settings):
        m1to1(-1,1),
        zto2pi(0,TWO_PI),
        R0(R0),
        settings(settings){
    }

    void generate(double & x, double & y, double & z){
        double phi = zto2pi(settings.GEN);
		double	u   = m1to1(settings.GEN);
        x = R0*cos(phi)*sqrt(1-u*u);
		y = R0*sin(phi)*sqrt(1-u*u);
		z = R0*u;   
    }

    std::string details(){
        return "\t\"source\" : \"uniform\",\n\t\"Ru\":" + std::to_string(R0);
    }
};