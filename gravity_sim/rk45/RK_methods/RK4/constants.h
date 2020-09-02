#include <vector>

struct RK4{
std::vector <std::vector<double>> a{{0,0,0,0},
                                    {0.5,0,0,0},
                                    {0,0.5,0,0},
                                    {0,0,1,0}};

std::vector <double> c{0, 0.5, 0.5, 1};
std::vector <double> b{1./6, 1./3, 1./3, 1./6};
};

