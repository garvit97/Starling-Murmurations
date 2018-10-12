#include <iostream>
#include "system.h"
#include <chrono>
#include "boid.h"
#include "predator.h"
#include <glm/glm.hpp>
#include <stdlib.h>
extern GLFWwindow* window;
extern void output(std::vector<boid>&, std::vector<predator>&, std::vector<glm::vec3>&);
extern void initializeEverything();
extern void  mouseButtonCallback(GLFWwindow*, int, int, int);
extern void print(string);
extern void key_callback(GLFWwindow*, int, int, int, int);
bool quit = false;
class system sys;
int main(int argc, char* argv[])
{
	int n=500;
	if(argc>1){
		n = std::atoi(argv[1]);
	}
	sys.startWithBoids(n);
	initializeEverything();
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, key_callback);
	while(!quit){
		output(sys.getBoids(),sys.getPredators(),sys.getObstacles());
		sys.updateSystem();
		usleep(10);
	}
}