#ifndef OBSTACLE_H
#define OBSTACLE_H

using namespace glm;

class obstacle
{
	vec3 position;
	vec3 velocity;
	vec3 prevVelocity;
	long prevPosTime;
	int flapStatus;

public:
	boid(vec3);//pos,default parameters
	boid(vec3,vec3);//pos,vel
	operator()();
	vec3 rule1and2();//consider boids as well as scavengers and stationary objects
	vec3 rule3();
	vec3 strongWind();
	vec3 tendToPlace();
	vec3 tendAwayFromPlace();//predators?
	void boundPosition(); //after every kind of velocity has been included,just before updating position.this will set the final velocity (change only that component where boundary is there)
	void limitVelocity();//normalize velocity acc to speed
	void moveBoidToNewPos();
	
	vec3 getPosition();
	void setPosition(vec3);
	vec3 getVelocity();
	void setVelocity(vec3);
	long getPrevPosTime();
	void setPrevPosTime(long);
	int getFlapStatus();
	int updateFlapStatus();
};

#endif