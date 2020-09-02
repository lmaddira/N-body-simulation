#include "carona_physics.h"

void collisions(vector<Body> &bodies, double CoR){ //vector of bodies, Coeff of restitution
    clock_t t1, t2;
    t1 = clock();

    #pragma omp parallel for
    for (auto i = 0; i < bodies.size(); i++){   
        auto b1 = bodies.begin()+i;
        auto r1 = b1->getPosition();
        auto s1 = b1->getRadius();

        #pragma omp parallel for
        for (auto j = 0; j < bodies.size(); j++){             
            if (j!=i){
                auto bn = bodies.begin()+j;   
                auto rn = bn->getPosition();
                auto sn = bn->getRadius();
                
                auto x = pow(abs(rn[0]-r1[0]),2);
                auto y = pow(abs(rn[1]-r1[1]),2);
                auto z = pow(abs(rn[2]-r1[2]),2);

                auto distance = pow(sqrt(x+y+z),2);

                if(distance<=(s1+sn)){
                    auto velocity=[](double m1,double m2,double v1,double v2, double CoR){
                        return (CoR*m2*(v2-v1)+m1*v1+m2*v2)/(m1+m2);
                    };
                    auto v1 = b1->getVelocity();
                    auto vn = bn->getVelocity();
                    auto m1 = b1->getMass();
                    auto mn = bn->getMass();
                
                    std::vector <double> new_v1(3);
                    std::vector <double> new_vn(3);
                    
                    #pragma omp parallel for
                    for(int a=0;a<3;a++){
                        new_v1[a]=velocity(m1,mn,v1[a],vn[a],CoR);
                        new_vn[a]=velocity(mn,m1,vn[a],v1[a],CoR);
                    }
                    b1->updateState(new_v1,r1);
                    bn->updateState(new_vn,rn);
                    b1->spread_carona(*bn);
                }
            }
        }
        //cout<<" ax "<<ax<<" ay "<<ay<<" az "<<az<<endl;
        t2 = clock();
        // cout<<" almost completed accel ...\n";
    }
    float time_taken = (float(t2-t1)/CLOCKS_PER_SEC);
    //cout<<time_taken<<endl;
}

void collisionsWithWalls(vector<Body> &bodies, double x0,double y0,double wid,double hei,double CoR){
    #pragma omp parallel for
    for (auto i = 0; i < bodies.size(); i++){
        auto b1 = bodies.begin()+i;
        auto r1 = b1->getPosition();
        auto s1 = b1->getRadius();
        auto v1 = b1->getVelocity();
        if((r1[0]+s1)>=(wid+x0) ||(r1[0]-s1)<=x0){
            v1[0]=-v1[0]*CoR;
        }
        if((r1[1]+s1)>=(hei+y0) ||(r1[1]-s1)<=y0){
            v1[1]=-v1[1]*CoR;
        }
        b1->updateState(v1,r1);

    }

}
