#include "boid.h"
#include "predator.h"
#include "system.h"
#include "params.h"

class system;

extern bool compareFloats(float, float); 
extern void printvec(vec3);
extern vec3 limitVelocity(vec3);//normalize velocity acc to speed
extern vec3 normalizeVelocity(vec3);

predator::predator(vec3 pos, class system *s)
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

predator::predator(class system *s)//pos,default parameters
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

predator::predator(vec3 pos,vec3 vel,class system *s)//pos,vel
{
	setPosition(pos);
	setVelocity(vel);
	setPrevVelocity(vel);
	setFlapStatus(rand()%flapStates);
	setPrevPosTime(chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count());
	setSystem(s);
}

vec3 predator::rule1()
{
	pair<vec3,vec3> neighbour=sys->findNearestBoid(getPosition());
	vec3 vel(0.0);
	vel=neighbour.first-getPosition();
	if(!compareFloats(length(vel),0.0))
		vel=normalizeVelocity(vel);
	return vel;
}

vec3 predator::rule3()
{
	vec3 c(0.0);
	pair<vector<pair<vec3,vec3>>,vector<pair<vec3,vec3>>> neighbours=sys->findObstaclesForPredator(getPosition());
	vector<pair<vec3,vec3>> neighbours2=neighbours.second;
	for(int i=0;i<neighbours2.size();++i)
	{
		c-=neighbours2[i].first-getPosition();
	}
	if(!compareFloats(length(c),0.0))
		c=normalizeVelocity(c);
	return c;
}

vec3 predator::strongWind()
{
	vec3 wind=sys->getWind();
	return wind;
}

vec3 predator::tendToPlace(vec3 place) 
{
	return (place - getPosition())/vec3(100.0);
}

vec3 predator::boundPosition(vec3 vel) 
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

void predator::movePredatorToNewPos()
{
	vec3 v1,v3;
	v1=rule1();
	v3=rule3();
	setPrevVelocity(getVelocity());
	vec3 vel=getPrevVelocity()+vec3(v1Mul)*v1+vec3(v3Mul)*v3;
	vel=limitVelocity(vel);
	vel=boundPosition(vel);
	setVelocity(vel);
	long currentTime=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	vec3 pos=getPosition()+getVelocity()*vec3((currentTime-getPrevPosTime())/(double)velocityDivision);
	setPosition(pos);
	setPrevPosTime(currentTime);
}

vec3 predator::getPosition()
{
	return position;
}

void predator::setPosition(vec3 position) 
{
	this->position=position;
}

vec3 predator::getVelocity()
{
	return velocity;
}

void predator::setVelocity(vec3 velocity)
{
	if(length(velocity)>maxSpeed)
		velocity=normalize(velocity)*vec3(maxSpeed);
	this->velocity=velocity;
}

vec3 predator::getPrevVelocity()
{
	return prevVelocity;
}

void predator::setPrevVelocity(vec3 v)
{
	prevVelocity=v;
}

long predator::getPrevPosTime()
{
	return prevPosTime;
}

void predator::setPrevPosTime(long prevPosTime)
{
	this->prevPosTime=prevPosTime;
}

int predator::getFlapStatus()
{
	return flapStatus;
}

void predator::setFlapStatus(int s)
{
	flapStatus=s;
}

int predator::updateFlapStatus()
{
	flapStatus=(flapStatus+1)%flapStates;
	return flapStatus;
}

void predator::setSystem(class system *s)
{
	sys=s;
}