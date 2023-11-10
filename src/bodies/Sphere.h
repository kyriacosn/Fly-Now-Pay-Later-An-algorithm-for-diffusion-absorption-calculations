#include <math.h>
#include <vector>
#include <random>

class Sphere{

    private:

	bool Is_meshed = false;
	std::vector<double> mesh,mesh_areas;
	std::vector<double> center;
	int n_states;

    public:
	
    Sphere(){}

    double const get_d(const double & x,const double & y,const double & z){
        return  sqrt(x*x + y*y + z*z) - 1;
    }

    std::vector<double> get_nearest_point(double const & x,double const & y,double const & z){
			
		double const r = sqrt(x*x+y*y+z*z);
		double const invr = 1/r;

		double const nx = invr*x;
		double const ny = invr*y;
		double const nz = invr*z;

		std::vector<double> nearest_point{nx,ny,nz};
		return nearest_point;	

    }

    double get_max_extend(){
		return 1.0;
	}

	void make_mesh(int N){

		double dz = 2.0/N;
		for (int i = 0;i<=N;i++){
			mesh.push_back(-1.0+i*dz);
		}

		for (int i = 0;i<N;i++){
			mesh_areas.push_back(2*M_PI*dz);
		}
		Is_meshed = true;
		n_states = N;
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

	std::vector<double> get_mesh(){
		return mesh;
	}

	// For multi body simulations////////////////

	void set_center(double x,double y,double z){
		center = {x,y,z};
	}

	std::vector<double> ref_coords(std::vector<double> coords){
        return {coords[0]-center[0],coords[1]-center[1],coords[2]-center[2]};
    }

	std::vector<double> orig_coords(std::vector<double> coords){
        return {coords[0]+center[0],coords[1]+center[1],coords[2]+center[2]};
    }
	
	///////////////////////////////////////////
    std::string details(){
		std::string det =  "\t\"body\" : \"sphere\"";
		if (Is_meshed==true){
       		std::string s = "[";
			for (int i =0;i<mesh.size()-1;i++){s +=  std::to_string(mesh[i]) + ",";}
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