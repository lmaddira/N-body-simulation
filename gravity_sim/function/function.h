#include <iostream>
#include <vector>
#include "body.h"

using namespace std;

void calcAcceleration(vector<Body> &bodies, double constant);

// kinematics vector of all the bodies concatinated would be sent to this function (x,y,z,xd,yd,zd)1,(x,y,z,xd,yd,zd)2.....
// as 6 variables are currently we are working on 

// std::vector<double> acceleration(std::vector<double> kinematicsvector,std::vector<double> mass,int bodynumber,double G){
// 	// we know the body number so we know position of it kinematicsvector
// 	double difference1,difference2,difference3, a1, a2, a3;
// 	std::vector<double> acceleration;
// 	for(int i = 1; i<=kinematicsvector.size()/6;i++){
// 		if(i!=bodynumber){
// 			difference1 = (kinematicsvector[6(i-1)]-kinematicsvector[6(bodynumber-1)]);
// 			difference2 = (kinematicsvector[6(i-1) +1]-kinematicsvector[6(bodynumber-1) +1]);
// 			difference3 = (kinematicsvector[6(i-1) +2]-kinematicsvector[6(bodynumber-1) +2]);
// 			a1+= (mass[i-1]*difference1/pow(abs(difference1),3));
// 			a2+= (mass[i-1]*difference2/pow(abs(difference2),3));
// 			a3+= (mass[i-1]*difference3/pow(abs(difference3),3));
// 		}	
// 	}
// 	acceleration.push_back(G*a1);
// 	acceleration.push_back(G*a2);
// 	acceleration.push_back(G*a3);
// 	return acceleration;
// }




// std::vector<double> function(std::vector<double> kinematicsvector, double G,std::vector<double> mass){ 
// 	int numofbodies = kinematicsvector.size()/6 ; // gives number of bodies
// 	int totalsize = kinematicsvector.size();
// 	std::vector<double> function;
// 	std::vector<double> accel;

// 	for(int i = 0; i<numofbodies;i++){
// 		if(i%6 == 0){
// 			function.push_back(kinematicsvector[i+3]); // position derivative is same as velocity thats given to the function 
// 			function.push_back(kinematicsvector[i+4]);
// 			function.push_back(kinematicsvector[i+5]);
// 			accel = acceleration(kinematicsvector,mass,(i+1),G);
// 			function.push_back(accel[0]);
// 			function.push_back(accel[1]);
// 			function.push_back(accel[2]);

// 		}
// 	}
// 	return function;
// } 




