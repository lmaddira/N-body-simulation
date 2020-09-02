#pragma once
#include <iostream>
#include <vector>

using namespace std;
enum state{
	normal,affected,cured
};

class Body{

private:

	vector <double> position;
	vector <double> velocity;
	vector <double> acceleration;

	double mass;
	double radius;
	state body_status;
	vector<int> time_to_cure_vec;
	

public:

	Body();
	int time_to_cure;
	Body(vector <double> velocity, vector <double> position, vector <double> acceleration, double mass, double radius,state body_status,int time);
	vector<double> getVelocity(void);
	vector<double> getPosition(void);
	vector<double> getAcceleration(void);
	double getMass(void);
	double getRadius(void);
	void updateState(vector <double> velocity, vector <double> position);
	void updateAcc(vector <double> acceleration);
	state get_status(void);
	void spread_carona(Body &b1);
	void spread(void);
	void update_status(void);	
	int get_time_to_cure(void);
	void update_time(int &time_to_cure);
	void cure(void);
};
