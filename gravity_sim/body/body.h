#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Body{

private:

	vector <double> position;
	vector <double> velocity;
	vector <double> acceleration;

	double mass;
	double radius;

public:

	Body();
	Body(vector <double> velocity, vector <double> position, vector <double> acceleration, double mass, double radius);
	vector<double> getVelocity(void);
	vector<double> getPosition(void);
	vector<double> getAcceleration(void);
	double getMass(void);
	double getRadius(void);
	void updateState(vector <double> velocity, vector <double> position);
	void updateAcc(vector <double> acceleration);
	
};
