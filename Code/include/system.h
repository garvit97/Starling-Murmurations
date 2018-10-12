#ifndef SYSTEM_H
#define SYSTEM_H
//#include "predator.h"
#include "params.h"

class boid;
class predator;

class system
{
	vector<class boid> boids;
	vector<class predator> predators;
	vector<vec3> obstacles;
	//float xmin,ymin,zmin,xmax,ymax,zmax;//2.4 each
	int numBoids;
	int numPredators;
	int numObstacles;
	bool windPresent;
	vec3 wind;
	//hyperparameters
	//float maxSpeed;//0.10
	//float minDistance;//for rule3
	//int flapStates;


public:
	system();
	void addBoid(class boid);
	void addBoid(vec3);
	void addBoid(vec3,vec3);
	void toggleEnergyOutput();
	void addPredator(class predator);
	void addPredator(vec3);
	void addPredator(vec3,vec3);
	void addObstacle(vec3);
	vector<pair<vec3,vec3>> findNeighbouringBoids(vec3);//their positions and velocities. look into ehat parameter to pass,maybe thread id or pointer to boid?
	pair<vector<pair<vec3,vec3>>,vector<pair<vec3,vec3>>> findVeryCloseObjects(vec3);
	pair<vec3,vec3> findNearestBoid(vec3);//for predator
	pair<vector<pair<vec3,vec3>>,vector<pair<vec3,vec3>>> findObstaclesForPredator(vec3);
	void startWithBoids(int);
	// float getXmin();
	// float getYmin();
	// float getZmin();
	// float getXmax();
	// float getYmax();
	// float getZmax();
	// float getMaxSpeed();
	int getNumBoids();
	void setNumBoids(int);
	int getNumPredators();
	void setNumPredators(int);
	int getNumObstacles();
	void setNumObstacles(int);
	// float getMinDistance();
	// void setMinDistance(float);
	void updateSystem();
	vector<class boid>& getBoids();
	vector<class predator>& getPredators();
	vector<vec3>& getObstacles();
	vec3 getWind();
	void setWind(vec3);
	bool getWindPresent();
	void setWindPresent(bool);
	// int getFlapStates();

};

#endif