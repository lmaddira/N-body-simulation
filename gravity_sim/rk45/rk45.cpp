#include "rk45.h"



Vec1d add(Vec1d a,Vec1d b){
    Vec1d r;
    for(int i=0;i<a.size();i++){
        r.push_back(a[i]+b[i]);        
    }
    return r;
}

Vec2d add(Vec2d a,Vec2d b){
    Vec2d r;
    for(int i=0;i<a.size();i++){
        Vec1d temp;

        for(int j=0;j<a[i].size();j++){
            temp.push_back(a[i][j]+b[i][j]);
        }
        r.push_back(temp);        
    }
    return r;
}


Vec1d subtract(Vec1d a,Vec1d b){
    Vec1d r;
    for(int i=0;i<a.size();i++){
        r.push_back(a[i]-b[i]);        
    }
    return r;
}

Vec2d subtract(Vec2d a,Vec2d b){
    Vec2d r;
    for(int i=0;i<a.size();i++){
        Vec1d temp;

        for(int j=0;j<a[i].size();j++){
            temp.push_back(a[i][j]-b[i][j]);
        }
        r.push_back(temp);        
    }
    return r;
}

Vec2d multiply(Vec2d a, double constant){
    Vec2d r;
    for(int i=0;i<a.size();i++){
        Vec1d temp;
        for(int j=0;j<a[i].size();j++){
            temp.push_back(a[i][j]*constant);
        }
        r.push_back(temp);        
    }
    return r;
}

void step(std::vector<Body> &bodies){

    double h=0.05;
    //saving position and velocity
    Vec2d r;
    Vec2d v;
    Vec2d r_new;
    Vec2d v_new;

    for (auto i = 0; i < bodies.size(); i++){
        auto b1 = bodies[i];
        r.push_back(b1.getPosition());       
        v.push_back(b1.getVelocity());
        r_new.push_back(b1.getPosition());       
        v_new.push_back(b1.getVelocity());

    }
      
    RK4 constants;
    Vec2d r_k(r.size() , Vec1d (r[0].size(), 0.));
    Vec2d v_k(v.size() , Vec1d (v[0].size(), 0.));
    
   
    for(int iter=0;iter<constants.a.size();iter++){
        //update accelerations
        calcAcceleration(bodies,10);
        //getting velocity and accelerations
        #pragma omp parallel for
        for (auto i = 0; i < bodies.size(); i++){
            auto b1 = bodies[i];
            r_k[i]=add(r_k[i],b1.getVelocity());
            v_k[i]=add(v_k[i],b1.getAcceleration());
        }
        Vec2d r_sum(r.size() , Vec1d (r[0].size(), 0.));
        Vec2d v_sum(v.size() , Vec1d (v[0].size(), 0.));

        for(int iter1=0;iter1<constants.a[iter].size();iter1++){
            //std::cout<<constants.a[iter][iter1]<<'\t';
            r_sum=add(multiply(r_k,constants.a[iter][iter1]*h),r_sum);
            v_sum=add(multiply(v_k,constants.a[iter][iter1]*h),v_sum);
        }
        //std::cout<<"\n";
        // Acutal update summation in position and velocity 
        r_new=add(r_new,multiply(r_k,constants.b[iter]*h));
        v_new=add(v_new,multiply(v_k,constants.b[iter]*h));
        
        
        // intermediate update in position and velocity;
        r=add(r,r_sum);
        v=add(v,v_sum);
        #pragma omp parallel for
        for (auto i = 0; i < bodies.size(); i++){
            (bodies.begin()+i)->updateState(v[i],r[i]);
        }
    }
    // Actual update in position and velocity
    #pragma omp parallel for
    for (auto i = 0; i < bodies.size(); i++){
        (bodies.begin()+i)->updateState(v_new[i],r_new[i]);
    }
}

double h=0.05;

void adaptiveStep(std::vector<Body> &bodies,double error){
    Cash_Karp constants;

    // error=0.01;
    double power=0.2;
    //saving position and velocity
    Vec2d r;
    Vec2d v;
    Vec2d r_new;
    Vec2d v_new;
    Vec2d r_new1;
    Vec2d v_new1;


    for (auto i = 0; i < bodies.size(); i++){
        auto b1 = bodies[i];
        r.push_back(b1.getPosition());       
        v.push_back(b1.getVelocity());
        r_new.push_back(b1.getPosition());       
        v_new.push_back(b1.getVelocity());
        r_new1.push_back(b1.getPosition());       
        v_new1.push_back(b1.getVelocity());
    }

    Vec2d r_k(r.size() , Vec1d (r[0].size(), 0.));
    Vec2d v_k(v.size() , Vec1d (v[0].size(), 0.));
       
    for(int iter=0;iter<constants.a.size();iter++){
        //update accelerations
        calcAcceleration(bodies,10);
        //getting velocity and accelerations
        #pragma omp parallel for
        for (auto i = 0; i < bodies.size(); i++){
            r_k[i]=add(r_k[i],bodies[i].getVelocity());
            v_k[i]=add(v_k[i],bodies[i].getAcceleration());
        }
        Vec2d r_sum(r.size() , Vec1d (r[0].size(), 0.));
        Vec2d v_sum(v.size() , Vec1d (v[0].size(), 0.));

        for(int iter1=0;iter1<constants.a[iter].size();iter1++){
            r_sum=add(multiply(r_k,constants.a[iter][iter1]*h),r_sum);
            v_sum=add(multiply(v_k,constants.a[iter][iter1]*h),v_sum);
        }
        // Acutal update summation in position and velocity 
        r_new=add(r_new,multiply(r_k,constants.b_[iter]*h));
        v_new=add(v_new,multiply(v_k,constants.b_[iter]*h));       
        r_new1=add(r_new1,multiply(r_k,constants.b[iter]*h));
        v_new1=add(v_new1,multiply(v_k,constants.b[iter]*h));       
        
        // intermediate update in position and velocity;
        r=add(r,r_sum);
        v=add(v,v_sum);
        
        #pragma omp parallel for
        for (auto i = 0; i < bodies.size(); i++){
            (bodies.begin()+i)->updateState(v[i],r[i]);
        }
    }

    #pragma omp parallel for
    for (auto i = 0; i < bodies.size(); i++){
        (bodies.begin()+i)->updateState(v[i],r[i]);
    }

    
    auto r_subtrated=subtract(r_new1,r_new);
    auto v_subtrated=subtract(v_new1,v_new);
    
    auto max=[](Vec2d a){
        Vec1d r;
        for(int i=0;i<a.size();i++){
            Vec1d temp;
            for(int j=0;j<a[i].size();j++){
                temp.push_back(fabs(a[i][j]));
            }
            r.push_back(*std::max_element(temp.begin(),temp.end()));        
        }
        double t=*std::max_element(r.begin(),r.end());
        return t;
    };





    double error1=max(r_subtrated);
    double error2=max(v_subtrated);
    error1=std::max(error1,error2);
    h=h*pow(error/error1,power);
    // std::cout<<"h: "<<h<<"\n";
    // if(h>0.01){
    //     h=0.05;
    // }
    // std::cout<<"Errors: "<<error1<<"\n";
    // std::cout<<"h: "<<h<<"\n";
    


    // Vec2d r_k(r.size() , Vec1d (r[0].size(), 0.));
    // Vec2d v_k(v.size() , Vec1d (v[0].size(), 0.));
       
    for(int iter=0;iter<constants.a.size();iter++){
        //update accelerations
        calcAcceleration(bodies,10);
        //getting velocity and accelerations
        #pragma omp parallel for
        for (auto i = 0; i < bodies.size(); i++){
            r_k[i]=add(r_k[i],bodies[i].getVelocity());
            v_k[i]=add(v_k[i],bodies[i].getAcceleration());
        }
        Vec2d r_sum(r.size() , Vec1d (r[0].size(), 0.));
        Vec2d v_sum(v.size() , Vec1d (v[0].size(), 0.));

        for(int iter1=0;iter1<constants.a[iter].size();iter1++){
            r_sum=add(multiply(r_k,constants.a[iter][iter1]*h),r_sum);
            v_sum=add(multiply(v_k,constants.a[iter][iter1]*h),v_sum);
        }
        // Acutal update summation in position and velocity 
        r_new=add(r_new,multiply(r_k,constants.b_[iter]*h));
        v_new=add(v_new,multiply(v_k,constants.b_[iter]*h));       
        
        // intermediate update in position and velocity;
        r=add(r,r_sum);
        v=add(v,v_sum);
        
        #pragma omp parallel for
        for (auto i = 0; i < bodies.size(); i++){
            (bodies.begin()+i)->updateState(v[i],r[i]);
        }
    }    
    
    // Actual update in position and velocity
    #pragma omp parallel for
    for (auto i = 0; i < bodies.size(); i++){
        (bodies.begin()+i)->updateState(v_new[i],r_new[i]);
    }
}

