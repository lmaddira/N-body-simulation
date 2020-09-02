#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fslazywindow.h>
#include "function.h"
// #include "physics.h"
// #include "rk45.h"

using namespace std;

class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;
    int num_bodies;
    vector <Body> bodies;
    vector <double> position;
    vector <double> velocity;
    vector <double> acceleration;
	std::vector <float> vtx,nom,col;
	double mass;
    double radius;
    double d,t;

	static void AddColor(std::vector <float> &col,float r,float g,float b,float a);
	static void AddVertex(std::vector <float> &vtx,float x,float y,float z);
    static void AddNormal(std::vector <float> &nom,float x,float y,float z);
    void getbodiesposition(vector <Body> &bodies,int &num_bodies,vector<float> &vtx);
public:
	FsLazyWindowApplication();
	virtual void BeforeEverything(int argc,char *argv[]);
	virtual void GetOpenWindowOption(FsOpenWindowOption &OPT) const;
	virtual void Initialize(int argc,char *argv[]);
	virtual void Interval(void);
	virtual void BeforeTerminate(void);
	virtual void Draw(void);
	virtual bool UserWantToCloseProgram(void);
	virtual bool MustTerminate(void) const;
	virtual long long int GetMinimumSleepPerInterval(void) const;
	virtual bool NeedRedraw(void) const;
};

/* static */ void FsLazyWindowApplication::AddColor(std::vector <float> &col,float r,float g,float b,float a)
{
	col.push_back(r);
	col.push_back(g);
	col.push_back(b);
	col.push_back(a);
}
/* static */ void FsLazyWindowApplication::AddVertex(std::vector <float> &vtx,float x,float y,float z)
{
	vtx.push_back(x);
	vtx.push_back(y);
	vtx.push_back(z);
}
/* static */ void FsLazyWindowApplication::AddNormal(std::vector <float> &nom,float x,float y,float z)
{
	nom.push_back(x);
	nom.push_back(y);
	nom.push_back(z);
}
void FsLazyWindowApplication::getbodiesposition(vector <Body> &bodies,int &num_bodies,vector<float> &vtx){
    for(auto body : bodies){
        vector<double> pos = body.getPosition();
        AddVertex(vtx,pos[0],pos[1],pos[2]);
    }
}

FsLazyWindowApplication::FsLazyWindowApplication()
{
	needRedraw=false;
}

/* virtual */ void FsLazyWindowApplication::BeforeEverything(int argc,char *argv[])
{
}
/* virtual */ void FsLazyWindowApplication::GetOpenWindowOption(FsOpenWindowOption &opt) const
{
	opt.x0=0;
	opt.y0=0;
	opt.wid=1200;
	opt.hei=800;
}
/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{	

    num_bodies = 3;
    d = 10; 
    // DO we need to initialise all ofthis here ?? why not use new Body to initiate all of these in Body constructor
    position.reserve(3);
    velocity.reserve(3);
    acceleration.reserve(3);

    velocity.push_back(0.0);
    velocity.push_back(0.0);
    velocity.push_back(0.0);

    position.push_back(0);
    position.push_back(0);
    position.push_back(0);

    acceleration.push_back(0.0);
    acceleration.push_back(0.0);
    acceleration.push_back(0.0);
    mass = 1.0; // can be changed to a different initial value
    radius = 1.0; // can be changed to a different initial value
    Body b1(velocity, position, acceleration, mass, radius);
    position.clear();
    position.push_back(0.5*100);
    position.push_back(0.866*100);
    position.push_back(0);

    Body b2(velocity, position, acceleration, mass, radius);
    position.clear();
    position.push_back(1.0*100);
    position.push_back(0);
    position.push_back(0);
    
    Body b3(velocity, position, acceleration, mass, radius);
    bodies.push_back(b1);
    bodies.push_back(b2);
    bodies.push_back(b3);



}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}
    //Calculating Accelerations
    auto G = 1.0; //Gravitational constt.
    calcAcceleration(bodies, G);
    for (auto i : bodies)
    {
        auto p = i.getPosition();
        auto a = i.getAcceleration();
        cout<<p[0]<<endl;
        cout<<p[1]<<endl;
        cout<<p[2]<<endl;
        cout<<"+++++++++"<<endl;
        cout<<a[0]<<endl;
        cout<<a[1]<<endl;
        cout<<a[2]<<endl;
        cout<<"========="<<endl;
    }
    auto a = bodies[0].getAcceleration();
    if(a[0]!=0.005||a[1]!=0.000133341|| a[2]!=0) std::cout<<"Error in acceleration calculation........\n";
    a = bodies[1].getAcceleration();
    if(a[0]!=0 || a[1]!= -0.000266682 || a[2]!=0) std::cout<<"Error in acceleration calculation........\n";
    a = bodies[2].getAcceleration();
    if(a[0]!=-0.005 || a[1]!= 0.000133341 || a[2]!=0) std::cout<<"Error in acceleration calculation........\n";


    vtx.clear();
	getbodiesposition(bodies,num_bodies,vtx);

	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	needRedraw=false;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	int wid,hei;
	FsGetWindowSize(wid,hei);
	auto aspect=(double)wid/(double)hei;
	glViewport(0,0,wid,hei);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0,wid,0,hei,0,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glEnable(GL_POINT_SMOOTH);
	glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(20);// later change it to function of radius
	// glEnableClientState(GL_COLOR_ARRAY);
	// glColorPointer(4,GL_FLOAT,0,col.data());
	glVertexPointer(3,GL_FLOAT,0,vtx.data());
	glDrawArrays(GL_POINTS,0,vtx.size()/3);
	glDisableClientState(GL_VERTEX_ARRAY);
	// glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_POINT_SMOOTH);
	FsSwapBuffers();
    SetMustTerminate(true);
}
/* virtual */ bool FsLazyWindowApplication::UserWantToCloseProgram(void)
{
	return true; // Returning true will just close the program.
}
/* virtual */ bool FsLazyWindowApplication::MustTerminate(void) const
{
	return FsLazyWindowApplicationBase::MustTerminate();
}
/* virtual */ long long int FsLazyWindowApplication::GetMinimumSleepPerInterval(void) const
{
	return 10;
}
/* virtual */ void FsLazyWindowApplication::BeforeTerminate(void)
{
}
/* virtual */ bool FsLazyWindowApplication::NeedRedraw(void) const
{
	return needRedraw;
}


static FsLazyWindowApplication *appPtr=nullptr;

/* static */ FsLazyWindowApplicationBase *FsLazyWindowApplicationBase::GetApplication(void)
{
	if(nullptr==appPtr)
	{
		appPtr=new FsLazyWindowApplication;
	}
	return appPtr;
}



/*
int main(){

    int num_bodies = 1000; // to be made argv later
    vector <Body> bodies;
    auto *bods = &bodies;

    for (int i = 0; i < num_bodies; i++)
    {
        vector <double> position;
		vector <double> velocity;
		vector <double> acceleration;

		double mass;
		double radius;

        position.reserve(3);
        velocity.reserve(3);
        acceleration.reserve(3);

        velocity.push_back(0.0);
        velocity.push_back(0.0);
        velocity.push_back(0.0);

        position.push_back(i);
        position.push_back(0);
        position.push_back(0);

        acceleration.push_back(0.0);
        acceleration.push_back(0.0);
        acceleration.push_back(0.0);

        mass = 1.0; // can be changed to a different initial value
        radius = 1.0; // can be changed to a different initial value
        Body b(velocity, position, acceleration, mass, radius);
        // Body b;
        bodies.push_back(b);
    }

    /* Three Body Test:  PASSED
    vector <double> position;
    vector <double> velocity;
    vector <double> acceleration;

    double mass;
    double radius;

    velocity.push_back(0.0);
    velocity.push_back(0.0);
    velocity.push_back(0.0);

    position.push_back(0.0);
    position.push_back(0.0);
    position.push_back(0.0);

    acceleration.push_back(0.0);
    acceleration.push_back(0.0);
    acceleration.push_back(0.0);

    mass = 1.0;
    radius = 1.0;

    Body b1(velocity, position, acceleration, mass, radius);
    position.clear();
    position.push_back(0.5);
    position.push_back(0.866);
    position.push_back(0);

    Body b2(velocity, position, acceleration, mass, radius);
    position.clear();
    position.push_back(1.0);
    position.push_back(0);
    position.push_back(0);
    
    Body b3(velocity, position, acceleration, mass, radius);
    bodies.push_back(b1);
    bodies.push_back(b2);
    bodies.push_back(b3);*/

/*
    //Calculating Accelerations
    auto G = 1.0; //Gravitational constt.


    calcAcceleration(bods, G);

    // for (auto i : bodies)
    // {
    //     auto p = i.getPosition();
    //     auto a = i.getAcceleration();
    //     cout<<p[0]<<endl;
    //     cout<<p[1]<<endl;
    //     cout<<p[2]<<endl;
    //     cout<<"+++++++++"<<endl;
    //     cout<<a[0]<<endl;
    //     cout<<a[1]<<endl;
    //     cout<<a[2]<<endl;
    //     cout<<"========="<<endl;
    // }
    // auto a = bodies[1].getAcceleration();
    cout<<bodies.size()<<endl;
    return 0;
}
*/
