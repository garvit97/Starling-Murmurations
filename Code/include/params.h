#include <thread>
#include<bits/stdc++.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;
using namespace glm;

#define xmin -2.4
#define ymin -1.8
#define zmin -2.4
#define xmax 2.4
#define ymax 1.8
#define zmax 2.4
#define maxSpeed 0.05 //1/20
#define minDistance 0.0235// (0.05+0.025/4.8)  //change acc to 0-1 scale. but 0-1 waala scale me birdspan ki values alag alag hongi x,y and z me bcoz x,y,z me wingspan same tha but x ,z 2.4 se 1 me scale ho rhe hain but y 1.8 se 1 me scale ho raha hai.
#define minDistanceForPredators 0.048
#define minDistanceForObstacles 0.20
#define flapStates 15
#define velocityDivision (maxSpeed*3000000)  //choose occurding to //cout
#define vMul 5
#define v1Mul 0.5
#define v2Mul 0.8
#define v3Mul 1.5
#define v4Mul 2.6
#define maxDistForRule1 0.5