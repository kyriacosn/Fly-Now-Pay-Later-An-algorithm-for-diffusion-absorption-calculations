#include <math.h>
#include <random>

class Point_source{

    private:
    
    double const xs,ys,zs;
    
    public:
    Point_source(double xs,double ys, double zs):
        xs(xs),
        ys(ys),
        zs(zs){
    }

    void generate(double & x,double & y,double & z){
        x = xs;
        y = ys;
        z = zs;
    }

    std::string details(){
        return "\t\"source\" : \"point\",\n\t\"x\" : " + std::to_string(xs)+
                                    ",\n\t\"y\" : " + std::to_string(ys)+
                                    ",\n\t\"z\" : " + std::to_string(zs);
    }

};