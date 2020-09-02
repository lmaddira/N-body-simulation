#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <omp.h>
#include <math.h>
#include <string>
#include <algorithm>
#include "carona_body.h"
// #include "function.h"

#include "../../gravity_sim/rk45/RK_methods/RK4/constants.h"
#include "../../gravity_sim/rk45/RK_methods/Cash_Karp/constants.h"
#include "../../gravity_sim/rk45/RK_methods/Dormand_Prince/constants.h"


typedef std::vector<double> Vec1d;
typedef std::vector<Vec1d> Vec2d;
typedef std::vector<Vec2d> Vec3d;



Vec2d add(Vec2d a,Vec2d b);
Vec2d subtract(Vec2d a,Vec2d b);

Vec2d multiply(Vec2d a, double constant);
void step(std::vector<Body> &bodies);
void adaptiveStep(std::vector<Body> &bodies,double error);

