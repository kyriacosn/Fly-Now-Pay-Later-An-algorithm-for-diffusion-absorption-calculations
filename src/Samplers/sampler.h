#include <math.h>
#include <vector>
#include <sstream>
#include <fstream>

class Sampler{
    private:

    std::ofstream tm_file,Nij_file;
    std::vector<std::vector<long int>> N;
    int n_states;
    int last_visit,new_visit;
    
    public:
    Sampler(int n_states):
        n_states(n_states),
        N(n_states + 2, std::vector<long int> (n_states + 2, 0)){}

    void initiate(std::string path){
        tm_file.open(path+"/transition_mtrx.dat", std::ios::out | std::ios::binary);
        Nij_file.open(path+"/Nij.dat", std::ios::out | std::ios::binary);

    }

    void sample(int index){
        double new_visit = index + 1;
        N[last_visit][new_visit] += 1;
        last_visit = new_visit;
    }

    void new_particle(){
        last_visit = 0;
    }

    void end_particle(){    
        N[last_visit][n_states + 1] ++;
    }

    void done(){

        std::vector<std::vector<double>> tr_mtrx (N.size(), std::vector<double> (N.size(), 0));

        for (size_t i = 0; i < N.size()-1; i++ ){
            long int sum = 0;
             for (auto& n : N[i])
                sum += n;
            
            for (size_t j = 0; j < N.size(); j++ ){
                tr_mtrx[i][j] = 1.0*N[i][j]/sum;
            }
        }

        tr_mtrx[tr_mtrx.size() -1][tr_mtrx.size() - 1] = 1;

        for(size_t i = 0; i < tr_mtrx.size(); i++ ){
            if ( tr_mtrx[i].size() > 0 ){
                const char* buffer = reinterpret_cast<const char*>(&tr_mtrx[i][0]);
                tm_file.write(buffer, tr_mtrx[i].size()*sizeof(double));
            }
        }    

        for(size_t i = 0; i < N.size(); i++ ){
            if ( N[i].size() > 0 ){
                const char* buffer = reinterpret_cast<const char*>(&N[i][0]);
                Nij_file.write(buffer, N[i].size()*sizeof(long int));
            }
        }    

    }

    std::string details(){
        return  "\t\"sampler\" : \"transition probabilities\"" ;
    }
};

