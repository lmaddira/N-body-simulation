#include <vector>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <fslazywindow.h>
#include "function.h"
//#include <GL\glut.h>
#include "physics.h"
#include "rk45.h"

using namespace std;

void drawCannonBall(float cx, float cy, float rad)
{
const float PI = 3.1415927;
glColor3f(1.0, 1.0, 1.0);
glBegin(GL_POLYGON);

for (int i = 0; i < 64; i++) {
float angle = (float)i * PI / 32.0;
float x = cx + cos(angle)*rad;
float y = cy + sin(angle)*rad;
glVertex2f(x, y);
}

glEnd();
}

class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;
    int num_bodies;
    vector <Body> bodies;
    vector <double> position;
    vector <double> velocity;
    vector <double> acceleration;
	std::vector <float> vtx,nom,col,trace,rad;
	double mass;
    double radius;
    double d,t;
    float G;
    int i;
    int count;
    float error;
    int mode;
    float CoR;

	static void AddColor(std::vector <float> &col,float r,float g,float b,float a);
	static void AddVertex(std::vector <float> &vtx,float x,float y,float z);
    static void AddNormal(std::vector <float> &nom,float x,float y,float z);
    void getbodiesposition(vector <Body> &bodies,int &num_bodies,vector<float> &vtx);
    void getbodiesposition(vector <Body> &bodies,int &num_bodies,vector<float> &vtx, vector<float> &trx);

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
void FsLazyWindowApplication::getbodiesposition(vector <Body> &bodies,int &num_bodies,vector<float> &vtx, vector<float> &trx){
    for(auto body : bodies){
        vector<double> pos = body.getPosition();
        AddVertex(vtx,pos[0],pos[1],pos[2]);
        AddVertex(trx,pos[0],pos[1],pos[2]);
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
    opt.useDoubleBuffer=1;
}
/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{	
	if(argc>2){
		num_bodies = atoi(argv[1]);
        mode=atoi(argv[2]);
        error=0.01;
        CoR=1.0;
        if (argc==3)
            error=atof(argv[3]);

        if (argc>4)
            CoR=atof(argv[4]);
        
        srand(time(0));
        //num_bodies = 3;
		d = 10; 
        int count = 0;
        // DO we need to initialise all ofthis here ?? why not use new Body to initiate all of these in Body constructor
        position.reserve(3);
        velocity.reserve(3);
        acceleration.reserve(3);

        // velocity.push_back(0.1*((rand()%9)-5));
        // velocity.push_back(0.1*((rand()%9)-5));
        // velocity.push_back(0.0);

        // position.push_back(600);
        // position.push_back(400);
        // position.push_back(0);

        acceleration.push_back(0.0);
        acceleration.push_back(0.0);
        acceleration.push_back(0.0);
        mass = 1.0; // can be changed to a different initial value
        radius = 1.0; // can be changed to a different initial value

        const float rad = 200;
        const float PI = 3.1415927;
        for (int i = 0; i < num_bodies; i++)
        {

            float angle = (float)i * PI / num_bodies*2;
            float x = cos(angle)*rad;
            float y = sin(angle)*rad;


            position.clear();
            double pos1 = ((rand()% 10000)/10000.0)*1000.0 + 100;//(rand()%num_bodies)*50+300;
            double pos2 = ((rand()% 10000)/10000.0)*600.0 + 100; //(rand()%num_bodies)*50+200;
            position.push_back(pos1);
            position.push_back(pos2);

            // position.push_back(x+600);
            // position.push_back(y+400);

            position.push_back(0);
            velocity.clear();
            velocity.push_back(((rand()%100)/10.0 -5.0)*0.5);
            velocity.push_back(((rand()%100)/10.0 -5.0)*0.5);

            // velocity.push_back(1.5*sin(angle));
            // velocity.push_back(1.5*cos(angle));

            // velocity.push_back(0.0);
            // velocity.push_back(0.0);

            velocity.push_back(0.0);
            mass = (rand()%10000)/1000.0 + 10;
            // mass = /*10*i/num_bodies8*/ + 10;

            radius = mass - 5;
            Body b(velocity, position, acceleration, mass, radius);
            bodies.push_back(b);
            // std::cout<<" pushing back bodies"<<i<<"\n";
            cout<<"mass: "<<mass<<" radius: "<<radius<<" pos 1 "<<pos1<<" pos 2 "<<pos2<<" vel 1 "<<velocity[0]<<" vel 2 "<<velocity[1]<<endl;
            
        }
        // position.clear();
        // position.push_back(600);
        // position.push_back(400);
        // position.push_back(0);
        // velocity.clear();
        // velocity.push_back(0);
        // velocity.push_back(0);
        // velocity.push_back(0.0);
        // mass = 1000000;
        // Body b(velocity, position, acceleration, mass, radius);
        // bodies.push_back(b);
        // std::cout<<" pushing back bodies"<<i<<"\n";
        // // cout<<"mass: "<<mass<<" pos 1 "<<pos1<<" pos 2 "<<pos2<<" vel 1 "<<velocity[0]<<" vel 2 "<<velocity[1]<<endl;
        
        // Body b1(velocity, position, acceleration, mass, radius);
        // position.clear();
        // position.push_back(600+0.5*100);
        // position.push_back(400+0.866*100);
        // position.push_back(0);
        // velocity.clear();
        // velocity.push_back(-0.5);
        // velocity.push_back(-0.86);
        // velocity.push_back(0.0);

        // Body b2(velocity, position, acceleration, mass, radius);
        // position.clear();
        // position.push_back(600+1.0*100);
        // position.push_back(400);
        // position.push_back(0);
        // velocity.clear();
        // velocity.push_back(-0.5);
        // velocity.push_back(0.86);
        // velocity.push_back(0.0);
        // mass = 10;
        // Body b3(velocity, position, acceleration, mass, radius);
        // bodies.push_back(b1);
        // bodies.push_back(b2);
        // bodies.push_back(b3);
        i = 0;

	}else{
		printf("Please provide the four required input arguments.........\n");
        printf("1: Number of Bodies:\n");
        printf("2: Time Stepping Mode:\n");
        printf("3: RK45 error:\n");
        printf("4: Coefficient of Restitution:\n");
		SetMustTerminate(true);
	}


}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}
    
    // if(i==0){
    if (mode==1)
        step(bodies);
    if (mode ==2)
        adaptiveStep(bodies,error);
    count++;
    
    // double CoR=0.95;
    collisions(bodies,CoR);
    collisionsWithWalls(bodies,50,50,1150,750,1.0);
    
    //     i = 1;
    // }

    if (count > 150)
    {
        for (int i = 0; i < 3*num_bodies; i++)
        {
            trace.erase(trace.begin());
        }
        //clear trace
        count--;
    }
    // getbodiesposition(bodies,num_bodies,trace);        
        
    vtx.clear();
	getbodiesposition(bodies,num_bodies,vtx,trace);
    // getbodiesradius(bodies,num_bodies,rad);    

	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	needRedraw=false;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0f);

	int wid,hei;
	FsGetWindowSize(wid,hei);
	auto aspect=(double)wid/(double)hei;
	glViewport(0,0,wid,hei);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0,wid,0,hei,0,1);
    //glTranslatef(0,0,-5);
    glColor3f(1,1,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//---------------------------------------------------
   /* glEnable(GL_POINT_SMOOTH);
	glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(10);// later change it to function of radius
	// glEnableClientState(GL_COLOR_ARRAY);
	// glColorPointer(4,GL_FLOAT,0,col.data());
	glVertexPointer(3,GL_FLOAT,0,vtx.data());
	glDrawArrays(GL_POINTS,0,vtx.size()/3);
	glDisableClientState(GL_VERTEX_ARRAY);
	// glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_POINT_SMOOTH);*/

    //--------------------Trace-----------------------------------//
    glEnable(GL_POINT_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(0.5);
	// glEnableClientState(GL_COLOR_ARRAY);
	// glColorPointer(4,GL_FLOAT,0,col.data());
    glColor3ub(100,100,100);
    glVertexPointer(3,GL_FLOAT,0,trace.data());
    glDrawArrays(GL_POINTS,0,trace.size()/3);
    // glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_POINT_SMOOTH);

    /*  --------------------------------------------------------*/
    
    for(auto i: bodies)
    {
        // cout<<i.getRadius()<<endl;
        //glPointSize(int(i.getRadius()));
        
        auto p = i.getPosition();
        auto r = i.getRadius();
        // glBegin(GL_POINTS);
        // // glEnable(GL_BLEND)
        // // glBegin(GL_POINTS);
        // glColor3f(1.0,1.0,1.0);
        // // glVertex3f(500.0f,300.0f,0.0f);
        // glVertex3f(p[0], p[1], p[2]);
        // glDisable(GL_BLEND);
        drawCannonBall(p[0], p[1], r*0.5);
        glEnd();    
    }
    
 
	FsSwapBuffers();
    //SetMustTerminate(true);
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
