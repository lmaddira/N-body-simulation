#include <math.h>
#include <iostream>
#include <time.h>
#include <omp.h>
#include <vector>
#include "body.h"
#include <math.h>

void collisions(vector<Body> &bodies, double CoR); //vector of bodies, Coeff of restitution
void collisionsWithWalls(vector<Body> &bodies, double x0,double y0,double wid,double hei,double CoR);