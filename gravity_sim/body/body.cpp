#include "body.h"
#include <iostream>

Body::Body(){ // constructor

	position.reserve(3);
	velocity.reserve(3);
	acceleration.reserve(3);

	velocity.push_back(0.0);
	velocity.push_back(0.0);
	velocity.push_back(0.0);

	position.push_back(0.0);
	position.push_back(0.0);
	position.push_back(0.0);

	acceleration.push_back(0.0);
	acceleration.push_back(0.0);
	acceleration.push_back(0.0);

	mass = 1.0; // can be changed to a different initial value
	radius = 1.0; // can be changed to a different initial value
}
Body::Body(vector <double> velocity, vector <double> position, vector <double> acceleration,double mass,double radius){ // initiating the Body poistion, velocities and mass
	

	this->position.reserve(3);
	this->velocity.reserve(3);
	this->acceleration.reserve(3);

	this->position.push_back(position[0]);
	this->position.push_back(position[1]);
	this->position.push_back(position[2]);

	this->velocity.push_back(velocity[0]);
	this->velocity.push_back(velocity[1]);
	this->velocity.push_back(velocity[2]);

	this->acceleration.push_back(acceleration[0]);
	this->acceleration.push_back(acceleration[1]);
	this->acceleration.push_back(acceleration[2]);

	this->mass = mass;
	this->radius = radius;
}
vector<double> Body::getPosition(void){ // returns vector of positions and velocities 

	return position;
}
vector<double> Body::getVelocity(void){ // returns vector of positions and velocities 
	
	return velocity;
}
vector<double> Body::getAcceleration(void){ // returns vector of positions and velocities 
	
	return acceleration;
}
double Body::getMass(void){
	return mass;
}
double Body::getRadius(void){
	return radius;
}
void Body::updateState(vector <double> velocity, vector <double> position){
	this->position[0] = position[0];
	this->position[1] = position[1];
	this->position[2] = position[2];

	this->velocity[0] = velocity[0];
	this->velocity[1] = velocity[1];
	this->velocity[2] = velocity[2];
}
void Body::updateAcc(vector <double> acceleration){

    //cout<<acceleration[0]<<endl;
	this->acceleration[0] = acceleration[0];
	this->acceleration[1] = acceleration[1];
	this->acceleration[2] = acceleration[2];
}
