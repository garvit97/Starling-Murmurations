#ifndef PREDATOR_H
#define PREDATOR_H
#include "params.h"
//using namespace glm;

class system;

class predator
{
	vec3 position;
	vec3 velocity;
	vec3 prevVelocity;
	long prevPosTime;
	int flapStatus;
	class system *sys;

public:
	predator(class system*);
	predator(vec3, class system *);//pos,default parameters
	predator(vec3,vec3,class system *);//pos,vel
	//operator()();
	vec3 rule1();//consider boids as well as scavengers and stationary objects
	vec3 rule3();//from obstacels and other predators
	vec3 strongWind();
	vec3 tendToPlace(vec3);
	vec3 tendAwayFromPlace();//predators?
	vec3 boundPosition(vec3); //after every kind of velocity has been included,just before updating position.this will set the final velocity (change only that component where boundary is there)
	//void limitVelocity();//normalize velocity acc to speed
	void movePredatorToNewPos();
	
	vec3 getPosition();
	void setPosition(vec3);
	vec3 getVelocity();
	void setVelocity(vec3);
	vec3 getPrevVelocity();
	void setPrevVelocity(vec3);
	long getPrevPosTime();
	void setPrevPosTime(long);
	int getFlapStatus();
	int updateFlapStatus();
	void setFlapStatus(int);
	void setSystem(class system *);
};

#endif