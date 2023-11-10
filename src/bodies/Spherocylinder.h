#include <math.h>
#include <vector>
#include <random>

class Spherocylinder{

    private:

    const double aspect_ratio;
    const double R;	
    const double Lc;
	std::vector<double> mesh,mesh_areas;
	bool Is_meshed;
	int n_states;


    public:

    Spherocylinder(double const aspect_ratio):
		aspect_ratio(aspect_ratio),
		R(1/sqrt(aspect_ratio)),
		Lc(R*(aspect_ratio - 1)),
		Is_meshed(false){
	}

	double const get_d(const double & x,const double & y,const double & z){
        if (z > Lc) 
			return sqrt( x*x + y*y + (z-Lc)*(z-Lc) ) - R;

		else if (z < - Lc) 
			return sqrt( x*x + y*y + (z+Lc)*(z+Lc) ) - R;

		else 
			return sqrt(x*x + y*y) - R;
	}

    std::vector<double> get_nearest_point(double const & x,double const & y,double const & z){

        double nx,ny,nz;

		if (z > Lc) {
			double const r = sqrt(x*x + y*y + (z-Lc)*(z-Lc));
			double const invr = R/r;	
			nx = invr*x;
			ny = invr*y;
			nz = invr*(z-Lc) + Lc;
		}

		else if (z < - Lc) {
			double const r = sqrt(x*x + y*y + (z+Lc)*(z+Lc));
			double const invr = R/r;
			nx = invr*x;
			ny = invr*y;
			nz = invr*(z+Lc) - Lc;
		}

		else {
			double const r = sqrt(x*x + y*y);
			double const invr = R/r;
			nx = invr*x;
			ny = invr*y;
			nz = z;
		}

		std::vector<double> nearest_point{nx,ny,nz};
		return nearest_point;	
    }

	double get_max_extend(){
		return Lc + R ;
	}

	
	void make_mesh(int N){
		
		double dA = 4*M_PI/N;
		double dz = dA/(2*M_PI*R);

		for (int i = 0;i<= N;i++){ mesh.push_back(-(Lc+R)+i*dz);}

		for (int i = 0;i<N;i++){ mesh_areas.push_back(2*M_PI*R*dz);}


        Is_meshed = true;
		n_states = N;
 
    };

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



	std::string details(){
		std::string det =  "\t\"body\" : \"spherocylinder\",\n\t\"aspect_ratio\" : " + std::to_string(aspect_ratio);
		det += ",\n\t\"R\" : " + std::to_string(R) + ",";
		det += "\n\t\"Lc\" : " + std::to_string(Lc);

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
