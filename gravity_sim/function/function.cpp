#include "function.h"
#include <math.h>
#include <iostream>
#include <time.h>
#include <omp.h>

using namespace std;

void calcAcceleration(vector<Body>  &bodies, double constant){

    clock_t t1, t2;

    t1 = clock();

    #pragma omp parallel for

    for (auto i = 0; i < bodies.size(); i++)
    {   
        auto b1 = bodies.begin()+i;

        vector <double> acc(3);

        double ax = 0.;
        double ay = 0.;
        double az = 0.;
        acc.clear();
        auto r1 = b1->getPosition();
        for (auto j = 0; j < bodies.size(); j++)
        {             
            if (j!=i)
            {
                auto bn = bodies.begin()+j;
                
                
                auto rn = bn->getPosition();

                auto x = pow(abs(rn[0]-r1[0]),2);
                auto y = pow(abs(rn[1]-r1[1]),2);
                auto z = pow(abs(rn[2]-r1[2]),2);

                // auto x3 = (x != 0) ? x:1.0;
                // auto y3 = (y != 0) ? y:1.0;
                // auto z3 = (z != 0) ? z:1.0;
                auto distance = pow(sqrt(x+y+z),3);

                ax += constant*bn->getMass()*(rn[0]-r1[0])/distance;
                ay += constant*bn->getMass()*(rn[1]-r1[1])/distance;
                az += constant*bn->getMass()*(rn[2]-r1[2])/distance;  
            }
        }
        //cout<<" ax "<<ax<<" ay "<<ay<<" az "<<az<<endl;
        float threshold = 1.0;
        ax = (ax > threshold) ? threshold:ax;
        ax = (ax < -threshold) ? -threshold:ax;
        ay = (ay > threshold) ? threshold:ay;
        ay = (ay < -threshold) ? -threshold:ay;
        az = (az > threshold) ? threshold:az;
        az = (az < -threshold) ? -threshold:az;

        acc.push_back(ax);
        acc.push_back(ay);
        acc.push_back(az);

        t2 = clock();
        // cout<<" almost completed accel ...\n";

        b1->updateAcc(acc);
    }
    float time_taken = (float(t2-t1)/CLOCKS_PER_SEC);
    //cout<<time_taken<<endl;

}
