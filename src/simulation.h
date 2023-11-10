#include <math.h>
#include <random>
#include <fstream>
#include <direct.h>


#define TWO_PI 6.283185307179586
#define PI 3.14159265359

template <typename Body,typename Source,typename Sampler>
class Simulation{

	public:
	Body body;
	Source source;
	Sampler& sampler;
	double const R0;
	std::string path;
	std::ofstream settings_file;
	std::ofstream enviroment_file;
	Settings & settings;


	Simulation(Body & body,Source & source, Sampler & sampler, Settings & settings,double const R0):
		body(body),
		source(source),
		sampler(sampler),
		R0(R0),
		settings(settings){
	}

	void setup(std::string path){

    	mkdir((path).c_str());

		sampler.initiate(path);

		settings_file.open(path+ "/settings.txt",std::ofstream::out | std::ofstream::app);
			settings_file << "{\n" + body.details() + ",\n";
			settings_file <<  source.details() + ",\n";
			settings_file << sampler.details() + ",\n";
			settings_file <<  settings.details() + ",\n\t\"R0\" : " + std::to_string(R0) + "\n}"; 
		settings_file.close();
	}

	void run(){


		std::uniform_real_distribution<double> urand(0.0,1.0),zto2pi(0.0,TWO_PI),m1to1(-1.0,1.0);
		std::normal_distribution<double> gauss(0.0,settings.LAMBDA/sqrt(2));

		double const R02  = R0*R0;
		double x,y,z,r;
		double phi,u,d,dc,r2;
		double r1,x_t,y_t,z_t,ct,st,cp,sp;
		double Pr;
		bool done;
		
		// Simulation loop
		for (int j = 0; j < settings.N_PARTICLES; j++){

			done = false;

			sampler.new_particle();

			source.generate(x,y,z);

			d = body.get_d(x,y,z);

			while (!done) {

				// Walk on Sphere
				if (d > settings.D0){
						
					phi = zto2pi(settings.GEN);
					u   = m1to1(settings.GEN);	
					x  += (d-settings.SW_C)*cos(phi)*sqrt(1-u*u);
					y  += (d-settings.SW_C)*sin(phi)*sqrt(1-u*u);	
					z  += (d-settings.SW_C)*u;

					r2  = x*x + y*y + z*z;

					// Still inside the outer Sphere
					if ( r2 < 1.1*R02){
						d = body.get_d(x,y,z);
					}
					// Gone outside
					else{
				
						r = sqrt(r2);
						Pr = R0/r;

						// Gone to infinity
						if (Pr < urand(settings.GEN)){
							sampler.end_particle();
							done = true;
						}

						// Return back to the outer sphere
						else{
							
							// Calculate new point around z-axis the escape point
							phi = zto2pi(settings.GEN);
							r1  = urand(settings.GEN);
							
							z_t = (2*r1*(Pr*Pr+1)*(Pr*(r1-1)+1)-(Pr-1)*(Pr-1))/
								(((2*r1-1)*Pr+1)*((2*r1-1)*Pr+1));	
							x_t = cos(phi)*sqrt(1-z_t*z_t);
							y_t = sin(phi)*sqrt(1-z_t*z_t);
										
							// Transform back 
							ct  = z/r;
							st  = sqrt(1.0-ct*ct);
							cp  = x/sqrt(x*x+y*y);
							sp  = cp*y/x;

							x   = R0*((x_t*ct + z_t*st)*cp - sp*y_t);
							y   = R0*((x_t*ct + z_t*st)*sp + cp*y_t);
							z   = R0*(- x_t*st + z_t*ct);
							
							d   = body.get_d(x,y,z);
						}
					}
				}

				// Brownian motion
				else {

					x += gauss(settings.GEN);
					y += gauss(settings.GEN);
					z += gauss(settings.GEN);	
					
					d = body.get_d(x,y,z);	

					//Inside the body
					if (d < 0){

						std::vector<double> np = body.get_nearest_point(x,y,z);

						sampler.sample(body.get_index(np));

						// Reflect
						x = 2*np[0] - x;
						y = 2*np[1] - y;
						z = 2*np[2] - z;

						d = body.get_d(x,y,z);
					}
				}
			}
		}

		sampler.done();
	}
};

