#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <cstring>
#include <fslazywindow.h>
#include </usr/include/GL/glut.h>
#include "carona_physics.h"
#include "carona_rk45.h"

using namespace std;
int random50()
{
    return rand() & 1;
}

int random75()
{
    return random50() | random50();
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
    vector <int> time_count;
	std::vector <float> vtx,nom,col;
	double mass;
    double radius;
    double d,t;
    float G;
    int i;
    int count;
    const char* data = "data.txt";
    FILE* fSol = fopen(data,"w");
    state body_status;
    int normalNo = 0;
    int affectedNo =0;
    int curedNo = 0;
    int start_infected;

	static void AddColor(std::vector <float> &col,float r,float g,float b,float a);
	static void AddVertex(std::vector <float> &vtx,float x,float y,float z);
    static void AddNormal(std::vector <float> &nom,float x,float y,float z);
    void getbodiesposition(vector <Body> &bodies,int &num_bodies,vector<float> &vtx);
    void get_bodies_color(vector<Body> &bodies,int &num_bodies,vector<float> &col,int &normalNo,int &affectedNo,int &curedNo);
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

void FsLazyWindowApplication::get_bodies_color(vector<Body> &bodies,int &num_bodies,vector<float> &col, int &normalNo,int &affectedNo,int &curedNo)
{
    normalNo = 0;
    affectedNo =0;
    curedNo =0;
    for(int b=0;b<bodies.size();b++){
        // body.update_status();
        // std::cout<<" updated status "<<bodies[b].get_time_to_cure()<<" time "<<time_count[b]<<"\n";
        auto status = bodies[b].get_status();
        if (status == normal)
        {
            AddColor(col,0,1,1,1);
            normalNo++;
        }
        else if (status == affected)
        {
            AddColor(col,1,0,0,1);
            affectedNo++;
            time_count[b] +=1; 
            if(time_count[b] > 1000)
            {
                bodies[b].cure();
                time_count[b] = 0;
            }
        }
        else if (status == cured)
        {
            AddColor(col,0,1,0,1);
            curedNo++;
        }
        else
        {
            std::cout<<"somethis is wrong \n";
            SetMustTerminate(true);
        }
        
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
	if(argc==3){
		num_bodies = atoi(argv[1]);
        start_infected = atoi(argv[2]);
        srand(time(0));
        //num_bodies = 3;
		d = 10; 
        // DO we need to initialise all ofthis here ?? why not use new Body to initiate all of these in Body constructor
        position.reserve(3);
        velocity.reserve(3);
        acceleration.reserve(3);

        acceleration.push_back(0.0);
        acceleration.push_back(0.0);
        acceleration.push_back(0.0);
        mass = 1.0; // can be changed to a different initial value
        radius = 1.0; // can be changed to a different initial value
        time_count.reserve(num_bodies);
        
        
        for (int i = 0; i < num_bodies; i++)
        {
            position.clear();
            double pos1 = ((rand()% 10000)/10000.0)*1200.0 ;//(rand()%num_bodies)*50+300;
            double pos2 = ((rand()% 10000)/10000.0)*800.0; //(rand()%num_bodies)*50+200;
            position.push_back(pos1);
            position.push_back(pos2);
            position.push_back(0);
            velocity.clear();
            velocity.push_back(((rand()%100)/10.0 -5.0)*2.0);
            velocity.push_back(((rand()%100)/10.0 -5.0)*2.0);
            velocity.push_back(0.0);
            mass = (rand()%100000)/1000.0+10;
            radius=25.;
            //time = 0; // always intiate it to 0
            if(i<start_infected) 
                body_status = affected;
            else
                body_status = normal; 
            Body b(velocity, position, acceleration, mass, radius,body_status,0);
            // (state body_status,int time)
            bodies.push_back(b);
            // std::cout<<" pushing back bodies"<<i<<"\n";
            // std::cout<<"mass: "<<mass<<" radius: "<<radius<<" pos 1 "<<pos1<<" pos 2 "<<pos2<<" vel 1 "<<velocity[0]<<" vel 2 "<<velocity[1]<<endl;
            time_count.push_back(0);
        }
        count = 0;
        /*
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
        i = 0;*/

	}else{
		printf("Please provide 3 arguments\n 1. executable ex: ./corona\n 2. number of bodies \n 3. number of infected bodies\n");
		SetMustTerminate(true);
	}


}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
        fclose(fSol);
		SetMustTerminate(true);
	}
    
    // if(i==0){
    step(bodies);
    // adaptiveStep(bodies,0.0000001);
    
    double CoR=1.0;
    collisions(bodies,CoR);
    collisionsWithWalls(bodies,-5,-5,1205,805,1);
    
    count++;
    // std::cout<<"iteration no "<<count<<"\n";
    vtx.clear();
	getbodiesposition(bodies,num_bodies,vtx);
    col.clear();
    get_bodies_color(bodies,num_bodies,col,normalNo,affectedNo,curedNo);
    fprintf(fSol, " %d   %d  %d \n",normalNo,affectedNo,curedNo);
	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	needRedraw=false;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.0,0.0,0.0,1.0f);
	int wid,hei;
	FsGetWindowSize(wid,hei);
	auto aspect=(double)wid/(double)hei;
	glViewport(0,0,wid,hei);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0,wid,0,hei,0,1);
    //glTranslatef(0,0,-5);
    glColor3f(0,0,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glEnable(GL_POINT_SMOOTH);
	glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(10);// later change it to function of radius
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4,GL_FLOAT,0,col.data());
	glVertexPointer(3,GL_FLOAT,0,vtx.data());
	glDrawArrays(GL_POINTS,0,vtx.size()/3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_POINT_SMOOTH);

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
