#include "carona_body.h"
#include <iostream>

int rand50(){
	return rand() & 1;
}
int rand75()
{
	return rand50() | rand50();
}


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
	this->body_status = normal; // can be changed to a different initial value
	// this->time_to_cure = 0;
}
Body::Body(vector <double> velocity, vector <double> position, vector <double> acceleration,double mass,double radius,state body_status,int time){ // initiating the Body poistion, velocities and mass
	

	this->position.reserve(3);
	this->velocity.reserve(3);
	this->acceleration.reserve(3);
	this->time_to_cure_vec.reserve(100);

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
	this->body_status = body_status;
	//this->time_to_cure = 0;
	this->time_to_cure = time;
	this->time_to_cure_vec.push_back(time) ;
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
state Body::get_status(void)
{
	return this->body_status;
}
int Body::get_time_to_cure(void)
{
	//return this->time_to_cure_vec.size();
	return this->time_to_cure;
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

void Body::spread_carona(Body &b1)
{
	auto b1_status = b1.get_status();
	if(rand75()) // include probability of affection as 25% for spread
	{// if only one the body is affected then spread carona
		if(this->body_status != b1_status && (this->body_status == affected || b1_status == affected))
		{			
			if(this->body_status == affected && b1_status != cured)
			{
				// std::cout<<"spreading carona this body is affected and other is normal \n";
				b1.spread();
			}else if(b1_status == affected && this->body_status !=cured)
			{
				// std::cout<<"spreading carona this is not affected other is afftected\n";
				this->spread();
			}
		}
	}
}
void Body::spread(void)
{
	this->body_status = affected;
	this->time_to_cure_vec.push_back(1);
	this->time_to_cure = this->time_to_cure + 1;
	return;

}
void Body::update_time(int &time_to_cure)
{
	this->time_to_cure_vec.push_back(1);
	this->time_to_cure = this->time_to_cure + 1;
	return;
}
void Body::update_status(void)
{
	if(this->body_status == affected)// if it's affected then update the time and check if it's time to change the status to cured
	{
		// std::cout<<" time to cure before "<< this->time_to_cure;
		//this->update_time(); 
		// std::cout<<" time to cure "<< this->time_to_cure;
		if(this->get_time_to_cure() > 9)
		{
			// std::cout<<"I'm cured \n";
			this->body_status = cured;
			this->time_to_cure_vec.clear();
			this->time_to_cure_vec.push_back(0);
		}else{
			this->update_time(this->time_to_cure);
		}
	}
	// if it's cured or not affected do nothing
}

void Body::cure(void)
{
	this->body_status = cured;
	// std::cout<<"I'm cured\n";
}

