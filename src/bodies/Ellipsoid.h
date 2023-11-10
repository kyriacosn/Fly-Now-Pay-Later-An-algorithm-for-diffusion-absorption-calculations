#include <math.h>
#include <vector>
#include <random>
#include <iostream>
#include "../../include/n_solve.h"


class Ellipsoid{

    private:

    const double aspect_ratio,er,ez;
    const double sqez,sqer,isqez,isqer;
	bool Is_meshed;
	std::vector<double> mesh,mesh_areas;
	int n_states;


    public:

    Ellipsoid(double const ar):
		aspect_ratio(ar),
		er((ar>1) ? sqrt(2.0/(1.0+ar*asin(sqrt(1.0-1.0/(ar*ar)))/sqrt(1.0-1.0/(ar*ar)))):
				sqrt(2.0/(1.0 + 0.5*ar*ar*log((1.0+sqrt(1.0-ar*ar))/(1.0-sqrt(1.0-ar*ar)))/sqrt(1.0-ar*ar)))),
		ez(er*ar),
		sqer(er*er),
		isqer(1/sqer),
		sqez(ez*ez),
		isqez(1/sqez),
		Is_meshed(false)
		{}

    double const get_d(const double & x,const double & y,const double & z){
		
		double const r2 = x*x + y*y;
		double const z2 =  z*z;
		double const t = 0.5*(z2*isqez + r2*isqer - 1)/(z2*isqez*isqez + r2*isqer*isqer);
		double const nz_f_m1 = sqez/(t+sqez)-1;
		double const nr_f_m1 = sqer/(t+sqer)-1;

        return ((t>0) - (t<0))*sqrt(z2*nz_f_m1*nz_f_m1 + r2*nr_f_m1*nr_f_m1);
    }

    std::vector<double> get_nearest_point(double & x,double & y,double & z){
	
		double const z2 = z*z;
		double const r2 = x*x + y*y;
		double const r = sqrt(r2);

		double const t = 0.5*(z2*isqez + r2*isqer - 1)/(z2*isqez*isqez + r2*isqer*isqer);

		double const nr = sqer*r/(t+sqer);
		double const nx = x*nr/r;
		double const ny = y*nr/r;
		double const nz = sqez*z/(t+sqez);
		std::vector<double> nearest_point{nx,ny,nz};
		return nearest_point;	
    }

	void make_mesh(int N){

		double ec = sqrt(ez*ez - er*er)/ez;
		double gamma = (4*ec)/(N*ez*er);

		double x0 = -ez;
		mesh.push_back(x0);

		double z0 = ec*x0/ez;
		for (int i =0;i<N;i++){

			auto fz = [z0,gamma] (double z) {
				return z*sqrt(1-z*z) + asin(z) - (z0*sqrt(1-z0*z0) + asin(z0)) - gamma;
			};

			double z1 = find_root(fz,z0,1,10000);

			mesh_areas.push_back(M_PI*(ez*er/ec)*(z1*sqrt(1-z1*z1) + asin(z1) - (z0*sqrt(1-z0*z0) + asin(z0))));
			mesh.push_back(ez*z1/ec);

			z0 = z1;

		}

        Is_meshed = true;
		n_states = N;
 
    };
	
	std::vector<double> get_mesh(){
		return mesh;
	}

    double get_max_extend(){
		return (ez>er) ? ez : er  ;
	}

	int get_index(std::vector<double> np){

        for (int i = 0;i<n_states;i++){
            if (np[2] <= mesh[i+1]){
                return i;
            }
        }
        return n_states-1;
    }


	int get_num_surface_states(){
		return n_states;
	}




    std::string details(){
		std::string det =  "\t\"body\" : \"ellipsoid\",\n\t\"aspect_ratio\" : " + std::to_string(aspect_ratio);
		det += ",\n\t\"ez\" : " + std::to_string(ez) + ",";
		det += "\n\t\"er\" : " + std::to_string(er);

		if (Is_meshed==true){
       		std::string s = "[";
			for (int i =0;i<mesh.size()-1;i++){ s +=  std::to_string(mesh[i]) + ",";}
			s += std::to_string(mesh.back());
        	s = s+ "]";
			det = det + ",\n\t\"mesh\" : "+  s;

       		s = "[";
			for (int i =0;i<mesh_areas.size()-1;i++){s +=  std::to_string(mesh_areas[i]) + ",";}
			s += std::to_string(mesh_areas.back());
        	s = s+ "]";
			det = det + ",\n\t\"mesh_area\" : " + s;
		} 
		return det ;
	}
};

