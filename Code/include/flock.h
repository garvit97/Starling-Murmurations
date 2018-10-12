#ifndef SYSTEM_H
#define SYSTEM_H

class system
{
	vector<boid*> boids;
	vector<predator*> predators;
	vector<obstacle*> obstacles;

public:
	flock();
	void addBoid(boid);
	void addPredator(predator);
	void addObstacle(obstacle);
	vector<vector<vec3>> findNeighbouringBoids(vec3);//their positions and velocities. look into ehat parameter to pass,maybe thread id or pointer to boid?

}

#endif