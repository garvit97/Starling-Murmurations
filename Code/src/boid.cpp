#include "boid.h"
#include "predator.h"
#include "system.h"
#include "params.h"

class system;

bool compareFloats(float A, float B) 
{
   float diff = A - B;
   float EPSILON=0.00001;
   return (diff < EPSILON) && (-diff < EPSILON);
}

void printvec(vec3 vv)
{
	cout<<vv[0]<<", "<<vv[1]<<", "<<vv[2]<<endl;
}

vec3 limitVelocity(vec3 v)//normalize velocity acc to speed
{
	if(length(v)>maxSpeed)
		v=normalize(v)*vec3(maxSpeed);
	return v;
}

vec3 normalizeVelocity(vec3 v)
{
	v=normalize(v)*vec3(maxSpeed);
	return v;
}

boid::boid(vec3 pos, class system *s)
{
	setPosition(pos);
	
	float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(maxSpeed/1.732)));
	float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(maxSpeed/1.732)));
	float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(maxSpeed/1.732)));
	setVelocity(vec3(x,y,z));
	setPrevVelocity(vec3(x,y,z));
	setFlapStatus(rand()%flapStates);
	setPrevPosTime(chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count());
	setSystem(s);
}

boid::boid(class system *s)//pos,default parameters
{
	float x1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	float y1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	float z1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	setPosition(vec3(x1,y1,z1));
	float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(maxSpeed/1.732)));
	float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(maxSpeed/1.732)));
	float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(maxSpeed/1.732)));
	setVelocity(vec3(x,y,z));
	setPrevVelocity(vec3(x,y,z));
	setFlapStatus(rand()%flapStates);
	setPrevPosTime(chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count());
	setSystem(s);
}

boid::boid(vec3 pos,vec3 vel,class system *s)//pos,vel
{
	setPosition(pos);
	setVelocity(vel);
	setPrevVelocity(vel);
	setFlapStatus(rand()%flapStates);
	setPrevPosTime(chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count());
	setSystem(s);
}

pair<vec3,vec3> boid::rule1and2()
{
	vector<pair<vec3,vec3>> neighbours=sys->findNeighbouringBoids(getPosition());
	long beg=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	vec3 pos(0.0),vel(0.0),v1(0.0),v2(0.0);
	for(int i=0;i<neighbours.size();++i)
	{
		pos+=neighbours[i].first;
		vel+=neighbours[i].second;
	}

	pos/=vec3(7.0);
	vel/=vec3(7.0);

	v1=pos-getPosition();
	
	if(!compareFloats(length(v1),0.0))
		v1=normalizeVelocity(v1); 

	v2=vel-getVelocity();
	
	if(!compareFloats(length(v2),0.0))
		v2=normalizeVelocity(v2);

	long end=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();

	return make_pair(v1,v2);
}

pair<vec3,vec3> boid::rule3()
{
	vec3 c1(0.0),c2(0.0);
	pair<vector<pair<vec3,vec3>>,vector<pair<vec3,vec3>>> neighbours=sys->findVeryCloseObjects(getPosition());
	long beg=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	vector<pair<vec3,vec3>> neighbours1=neighbours.first, neighbours2=neighbours.second;
	for(int i=0;i<neighbours1.size();++i)
	{
		c1-=neighbours1[i].first-getPosition();
	}

	for(int i=0;i<neighbours2.size();++i)
	{
		c2-=neighbours2[i].first-getPosition();
	}
	if(!compareFloats(length(c1),0.0))
		c1=normalizeVelocity(c1);
	if(!compareFloats(length(c2),0.0))
		c2=normalizeVelocity(c2);

	pair<vec3,vec3> c=make_pair(c1,c2);
	long end=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	return c;
}

vec3 boid::strongWind()
{
	vec3 wind=sys->getWind();
	return wind;
}

vec3 boid::tendToPlace(vec3 place) 
{
	return (place - getPosition())/vec3(100.0);
}

vec3 boid::boundPosition(vec3 vel)
{
	vec3 pos=getPosition();

	if(pos[0]<=0+minDistance)	vel[0]=10*maxSpeed;
	else if(pos[0]>=1-minDistance)	vel[0]=-10*maxSpeed;

	if(pos[1]<=0+minDistance)	vel[1]=10*maxSpeed;
	else if(pos[1]>=1-minDistance)	vel[1]=-10*maxSpeed;

	if(pos[2]<=0+minDistance)	vel[2]=10*maxSpeed;
	else if(pos[2]>=1-minDistance)	vel[2]=-10*maxSpeed;
	return vel;
}

void boid::moveBoidToNewPos()
{

	vec3 v1,v2,v3,v4;
	pair<vec3,vec3> v1v2=rule1and2();
	v1=v1v2.first; v2=v1v2.second;
	pair<vec3,vec3> v3v4=rule3();
	long beg=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	v3=v3v4.first; v4=v3v4.second;
	setPrevVelocity(getVelocity());
	vec3 vel=vec3(vMul)*getPrevVelocity()+vec3(v1Mul)*v1+vec3(v2Mul)*v2+vec3(v3Mul)*v3+vec3(v4Mul)*v4;
	vel=limitVelocity(vel);
	vel=boundPosition(vel);
	setVelocity(vel);
	long currentTime=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	vec3 pos=getPosition()+getVelocity()*vec3((currentTime-getPrevPosTime())/(double)velocityDivision);
	setPosition(pos);
	setPrevPosTime(currentTime);
	long end=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}

vec3 boid::getPosition()
{
	return position;
}

void boid::setPosition(vec3 position) 
{
	this->position=position;
}

vec3 boid::getVelocity()
{
	return velocity;
}

void boid::setVelocity(vec3 velocity)
{
	if(length(velocity)>maxSpeed)
		velocity=normalize(velocity)*vec3(maxSpeed);
	this->velocity=velocity;
}

vec3 boid::getPrevVelocity()
{
	return prevVelocity;
}

void boid::setPrevVelocity(vec3 v)
{
	prevVelocity=v;
}

long boid::getPrevPosTime()
{
	return prevPosTime;
}

void boid::setPrevPosTime(long prevPosTime)
{
	this->prevPosTime=prevPosTime;
}

int boid::getFlapStatus()
{
	return flapStatus;
}

void boid::setFlapStatus(int s)
{
	flapStatus=s;
}

int boid::updateFlapStatus()
{
	flapStatus=(flapStatus+1)%flapStates;
	return flapStatus;
}

void boid::setSystem(class system *s)
{
	sys=s;
}