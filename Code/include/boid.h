#ifndef BOID_H
#define BOID_H
#include "params.h"
//using namespace glm;

class system;

class boid
{
	vec3 position;
	vec3 velocity;
	vec3 prevVelocity;
	long prevPosTime;
	int flapStatus;
	class system *sys;

public:
	boid(class system*);
	boid(vec3,class system *);//pos,default parameters
	boid(vec3,vec3,class system *);//pos,vel
	//void operator()();
	pair<vec3,vec3> rule1and2();//consider boids as well as scavengers and stationary objects
	pair<vec3,vec3> rule3();
	vec3 strongWind();
	vec3 tendToPlace(vec3);
	vec3 tendAwayFromPlace();//predators?
	//void limitVelocity();//normalize velocity acc to speed
	void moveBoidToNewPos();
	vec3 boundPosition(vec3);

	vec3 getPosition();
	void setPosition(vec3);
	vec3 getVelocity();
	void setVelocity(vec3);
	vec3 getPrevVelocity();
	void setPrevVelocity(vec3);
	long getPrevPosTime();
	void setPrevPosTime(long);
	int getFlapStatus();
	void setFlapStatus(int);
	int updateFlapStatus();
	void setSystem(class system *);
};

#endif