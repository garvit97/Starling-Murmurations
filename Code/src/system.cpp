#include "system.h"
#include "boid.h"
#include "predator.h"
#include <bits/stdc++.h>
using namespace std;
bool printEnergy;

void system::toggleEnergyOutput()
{
	printEnergy=1-printEnergy;
}

float distance(vec3 a,vec3 b)
{
	return length(a-b);
}

system::system()
{
	setNumBoids(0);
	printEnergy =false;
}

void system::addBoid(boid b)
{
	boids.push_back(b);
	setNumBoids(getNumBoids()+1);
}

void system::addBoid(vec3 pos)
{
	boids.push_back(boid(pos,this));
	setNumBoids(getNumBoids()+1);
}

void system::addBoid(vec3 pos,vec3 vel)
{
	boids.push_back(boid(pos,vel,this));
	setNumBoids(getNumBoids()+1);
}

void system::addPredator(predator p)
{
	predators.push_back(p);
	setNumPredators(getNumPredators()+1);
}

void system::addPredator(vec3 pos)
{
	predators.push_back(predator(pos,this));
	setNumPredators(getNumPredators()+1);
}

void system::addPredator(vec3 pos, vec3 vel)
{
	predators.push_back(predator(pos,vel,this));
	setNumPredators(getNumPredators()+1);
}


void system::addObstacle(vec3 o)
{
	obstacles.push_back(o);
	setNumObstacles(getNumObstacles()+1);
}


vector<pair<vec3,vec3>> system::findNeighbouringBoids(vec3 b)
{
	long beg=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	vector<pair<vec3,vec3>> neighbours;
	vector<float> dist;

	for(int i=0;i<boids.size();++i)
	{
		dist.push_back(distance(b,boids[i].getPosition()));
	}
	
	for(int i=0;i<7;++i)
	{
		float  minimum=dist[i]; int minindex=i;
		for(int j=i+1;j<boids.size();++j)
		{
			if(dist[j]<minimum)
			{
				minimum=dist[j];
				minindex=j;
			}
		}
		if(minimum>maxDistForRule1)	break;
		boid temp=boids[minindex];
		float temp1=dist[minindex];
		boids[minindex]=boids[i];
		dist[minindex]=dist[i];
		boids[i]=temp;
		dist[i]=temp1;
		neighbours.push_back(make_pair(boids[i].getPosition(),boids[i].getVelocity()));
	}
	long end=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	return neighbours;
}

pair<vector<pair<vec3,vec3>>,vector<pair<vec3,vec3>>> system::findVeryCloseObjects(vec3 b)
{
	long beg= chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	vector<pair<vec3,vec3>> closeObjects1,closeObjects2;
	for(int i=0;i<boids.size();++i)
	{
		float dist=distance(b,boids[i].getPosition());
		if(dist<=(0.5*minDistance))
		{
			closeObjects1.push_back(make_pair(boids[i].getPosition(),boids[i].getVelocity()));
		}

		else if(dist<=minDistance)
		{
			closeObjects2.push_back(make_pair(boids[i].getPosition(),boids[i].getVelocity()));
		}	
	}	

	for(int i=0;i<predators.size();++i)
	{
		float dist=distance(b,predators[i].getPosition());
		if(dist<=(0.5*minDistanceForPredators))
			closeObjects1.push_back(make_pair(predators[i].getPosition(),predators[i].getVelocity()));

		else if(dist<=minDistanceForPredators)
			closeObjects2.push_back(make_pair(predators[i].getPosition(),predators[i].getVelocity()));
	}

	for(int i=0;i<obstacles.size();++i)
	{
		float dist=distance(b,obstacles[i]);
		if(dist<=(0.5*minDistanceForObstacles))
			closeObjects1.push_back(make_pair(obstacles[i],obstacles[i]));

		else if(dist<=minDistanceForObstacles)
			closeObjects2.push_back(make_pair(obstacles[i],obstacles[i]));
	}

	pair<vector<pair<vec3,vec3>>,vector<pair<vec3,vec3>>> closeObjects=make_pair(closeObjects1,closeObjects2);
	long end= chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	return closeObjects;
}

pair<vector<pair<vec3,vec3>>,vector<pair<vec3,vec3>>> system::findObstaclesForPredator(vec3 b)
{
	vector<pair<vec3,vec3>> closeObjects1,closeObjects2;

	for(int i=0;i<predators.size();++i)
	{
		if(distance(b,predators[i].getPosition())<=(0.7*minDistanceForPredators))
			closeObjects1.push_back(make_pair(predators[i].getPosition(),predators[i].getVelocity()));

		else if(distance(b,predators[i].getPosition())<=(1.4*minDistanceForPredators))
			closeObjects2.push_back(make_pair(predators[i].getPosition(),predators[i].getVelocity()));
	}

	for(int i=0;i<obstacles.size();++i)
	{
		if(distance(b,obstacles[i])<=(0.7*minDistanceForObstacles))
			closeObjects1.push_back(make_pair(obstacles[i],obstacles[i]));

		else if(distance(b,obstacles[i])<=(1.4*minDistanceForObstacles))
			closeObjects2.push_back(make_pair(obstacles[i],obstacles[i]));
	}

	pair<vector<pair<vec3,vec3>>,vector<pair<vec3,vec3>>> closeObjects=make_pair(closeObjects1,closeObjects2);
	return closeObjects;
}

pair<vec3,vec3> system::findNearestBoid(vec3 p)
{
	float mini=0;
	for(int i=1;i<boids.size();++i)
	{
		if(distance(p,boids[i].getPosition())<distance(p,boids[mini].getPosition()))
		{
			mini=i;
		}
	}
	return make_pair(boids[mini].getPosition(),boids[mini].getVelocity());
}

void system::startWithBoids(int n)
{
	if(n>400)	n=400+(n-400)*0.7;
	int maxSize=2000;
	int arr1[2000],arr2[2000],arr3[2000];
	for(int i=0;i<maxSize;++i)
	{
		arr1[i]=i;
		arr2[i]=i;
		arr3[i]=i;
	}
	int max=maxSize-1;
	for(int i=max;i>max-n;--i)
	{
		int r;
		bool newFound=false;
		do
		{
			newFound=false;
			r=rand()%i;
			if(arr1[r]==r)
			{
				arr1[i]=r;
				arr1[r]=i;
				newFound=true;
			}
		}while(!newFound);

		do
		{
			newFound=false;
			r=rand()%i;
			if(arr2[r]==r)
			{
				arr2[i]=r;
				arr2[r]=i;
				newFound=true;
			}
		}
		while(!newFound);
		do
		{
			newFound=false;
			r=rand()%i;
			if(arr3[r]==r)
			{
				arr3[i]=r;
				arr3[r]=i;
				newFound=true;
			}
		}
		while(!newFound);	
	}

	for(int i=max;i>max-n;--i)
	{
		float x=((float)arr1[i]/max)*(1-2*minDistance)+minDistance;
		float y=((float)arr2[i]/max)*(1-2*minDistance)+minDistance;
		float z=((float)arr3[i]/max)*(1-2*minDistance)+minDistance;
		addBoid(vec3(x,y,z));
	}	
}

void system::setNumBoids(int n)
{
	numBoids=n;
}

int system::getNumBoids()
{
	return numBoids;
}

int system::getNumPredators()
{
	return numPredators;
}

void system::setNumPredators(int n)
{
	numPredators=n;
}

int system::getNumObstacles()
{
	return numObstacles;
}

void system::setNumObstacles(int n)
{
	numObstacles=n;
}

void system::updateSystem()
{
	//boids
	float energy=0.0;//70g
	for(int i=0;i<boids.size();++i)
	{
		boids[i].moveBoidToNewPos();
	}
	if(printEnergy)
	{
		for(int i=0;i<boids.size();++i)
		{
			vec3 vel=boids[i].getVelocity();
			float velocity=(length(vel)/maxSpeed)*40;
			energy+=0.035*velocity*velocity;
		}

		for(int i=0;i<1&&i<predators.size();++i)
		{
			vec3 vel=predators[i].getVelocity();
			float velocity=(length(vel)/maxSpeed)*85;
			energy+=0.05*velocity*velocity;
		}

		energy/=(boids.size()+1);
		cout<<"Average Energy of boids:"<<energy<<endl;	
	}
	
	//predators
	for(int i=0;i<predators.size();++i)
	{
		predators[i].movePredatorToNewPos();
	}
}

vec3 system::getWind()
{
	return wind;
}

void system::setWind(vec3 w)
{
	wind=w;
	if(w!=vec3(0.0))	setWindPresent(true);
}

bool system::getWindPresent()
{
	return windPresent;
}

void system::setWindPresent(bool wp)
{
	windPresent=wp;
	if(!wp)	setWind(vec3(0.0));
}

vector<boid>& system::getBoids()
{
	return boids;
}

vector<class predator>& system::getPredators()
{
	return predators;
}

vector<vec3>& system::getObstacles()
{
	return obstacles;
}