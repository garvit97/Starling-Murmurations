#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
GLFWwindow* window;
#include "shader.h"
#include <unistd.h>
#include <vector>
#include <iostream>
#include <math.h>
#include "system.h"
#include "boid.h"
#include "params.h"
#include "predator.h"
extern class system sys;
extern bool quit;
std::vector<glm::vec3> vertices_flap2{glm::vec3(0,1.5,-0.102471),glm::vec3(-0.5,-0.5,-0.390471),glm::vec3(0.5,-0.5,-0.390471),glm::vec3(0,-0.5,0.475529),glm::vec3(0,-1.5,-0.102471),glm::vec3(1.4,0.5,1.0295),glm::vec3(1.77,0,1.1266),glm::vec3(1.77,0,1.0665),glm::vec3(-1.4,0.5,1.0295),glm::vec3(-1.77,0,1.1266),glm::vec3(-1.77,0,1.0295),glm::vec3(-0.0415631,0.75,0.0425294),glm::vec3(-0.150781,-0.25,0.142529),glm::vec3(-0.266067,-0.25,-0.0574706),glm::vec3(0.0415631,0.75,0.0425294),glm::vec3(0.150781,-0.25,0.142529),glm::vec3(0.266067,-0.25,-0.0574706)};
std::vector<glm::vec3> vertices_flap1{glm::vec3(0,1.5,-0.102471),glm::vec3(-0.5,-0.5,-0.390471),glm::vec3(0.5,-0.5,-0.390471),glm::vec3(0,-0.5,0.475529),glm::vec3(0,-1.5,-0.102471),glm::vec3(1.4,0.5,-0.944471),glm::vec3(1.77,0,-0.981471),glm::vec3(1.77,0,-1.18147),glm::vec3(-1.4,0.5,-0.944471),glm::vec3(-1.77,0,-0.981471),glm::vec3(-1.77,0,-1.18147),glm::vec3(-0.0415631,0.75,0.0425294),glm::vec3(-0.150781,-0.25,0.142529),glm::vec3(-0.266067,-0.25,-0.0574706),glm::vec3(0.0415631,0.75,0.0425294),glm::vec3(0.150781,-0.25,0.142529),glm::vec3(0.266067,-0.25,-0.0574706)};
std::vector<uint16_t> indices_flap{0,2,1,4,3,1,4,2,3,4,1,2,15,5,14,15,6,5,14,5,16,16,5,7,9,11,8,9,12,11,10,8,11,10,11,13,10,13,12,9,10,12,16,7,6,15,16,6,3,15,0,15,14,0,14,16,0,0,16,2,14,15,16,3,16,15,3,2,16,0,1,12,0,12,11,12,1,13,11,12,13,13,1,3,0,11,13,0,13,3,5,6,7,9,8,10};

std::vector<glm::vec3> vertices_obstacle{glm::vec3(-1,-1,1),glm::vec3(1,-1,1),glm::vec3(1,1,1),glm::vec3(-1,1,1),glm::vec3(-1,-1,-1),glm::vec3(1,-1,-1),glm::vec3(1,1,-1),glm::vec3(-1,1,-1)};
std::vector<uint16_t> indices_obstacle{0,1,3,3,1,2,2,1,5,2,5,6,3,2,7,7,2,6,3,7,4,3,4,0,0,4,1,4,5,1,4,7,6,5,4,6};
std::vector<glm::vec3> normals_obstacle{glm::vec3(-1,-1,1),glm::vec3(1,-1,1),glm::vec3(1,1,1),glm::vec3(-1,1,1),glm::vec3(-1,-1,-1),glm::vec3(1,-1,-1),glm::vec3(1,1,-1),glm::vec3(-1,1,-1)};

GLuint vertexbuffer_flap2, elementbuffer_flap, vertexbuffer_flap1, elementbuffer_obstacle, vertexbuffer_obstacle, normalbuffer_obstacle, programID, programID_lighting, ModelMatrixID, ModelMatrixID_lighting, ViewMatrixID, ViewMatrixID_lighting, MatrixID, MatrixID_lighting, LightID, vertexPosID, vertexPosID_ligthing, vertexNormalID, vertexColorID, vertexColorID_lighting, scalingFactorID, scalingFactorID_lighting;

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 rotationMatrixFrame;

float FoV = 45.0f;
// Initial position : on +Z
glm::vec3 position = glm::vec3(0,0,5); 

glm::mat4 rotMatrix = glm::mat4(1.0f);

// Initial horizontal angle
float angle_y = 0.0f;
// Initial vertical angle
float angle_x = 0.0f;
//Speed of rotation
float Speed = 0.01f;

bool allow = true, key_allow = true;

glm::vec3 lightPos = glm::vec3(0,0,25);
glm::vec3 predatorColor = glm::vec3(0.41,0.41,0.41);
glm::vec3 boidColor = glm::vec3(0.078f,0.078f,0.078f);
glm::vec3 obstacleColor = glm::vec3(0.277,0.56,0);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS && key_allow==true){
        sys.toggleEnergyOutput();
        key_allow = false;
    }
    if (key == GLFW_KEY_E && action == GLFW_RELEASE && key_allow==false)
    	key_allow = true;

}
glm::mat4 computeRotationOfFrame(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		angle_x = -Speed*deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		angle_y = Speed*deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		angle_x = Speed*deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		angle_y = -Speed*deltaTime;
	}
	glm::vec3 myRotationAxis_x(1,0,0);
	glm::mat4 rotationMatrixX = glm::rotate(float(angle_x*180.0f/M_PI), myRotationAxis_x);
	glm::vec3 myRotationAxis_y(0,1,0);
	glm::mat4 rotationMatrixY = glm::rotate(float(angle_y*180.0f/M_PI), myRotationAxis_y);
	rotMatrix= rotationMatrixX*rotationMatrixY*rotMatrix;
	angle_x = 0;
	angle_y = 0;

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
	return rotMatrix;
}

void initializeWindow(){
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Starling Murmuration", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
}
void setObjectID(){
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );
	programID_lighting = LoadShaders( "StandardShading_lighting.vertexshader", "StandardShading_lighting.fragmentshader" );
	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "P");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	MatrixID_lighting = glGetUniformLocation(programID_lighting, "P");
	ViewMatrixID_lighting = glGetUniformLocation(programID_lighting, "V");
	ModelMatrixID_lighting = glGetUniformLocation(programID_lighting, "M");
}
void createBuffers(){
	//Create buffers in graphic card

	glGenBuffers(1, &vertexbuffer_flap1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_flap1);
	glBufferData(GL_ARRAY_BUFFER, vertices_flap1.size() * sizeof(glm::vec3), &vertices_flap1[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer_flap);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_flap);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_flap.size() * sizeof(int16_t), &indices_flap[0] , GL_STATIC_DRAW);

	glGenBuffers(1, &vertexbuffer_flap2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_flap2);
	glBufferData(GL_ARRAY_BUFFER, vertices_flap2.size() * sizeof(glm::vec3), &vertices_flap2[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vertexbuffer_obstacle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obstacle);
	glBufferData(GL_ARRAY_BUFFER, vertices_obstacle.size() * sizeof(glm::vec3), &vertices_obstacle[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer_obstacle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_obstacle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_obstacle.size() * sizeof(int16_t), &indices_obstacle[0] , GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer_obstacle);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_obstacle);
	glBufferData(GL_ARRAY_BUFFER, normals_obstacle.size() * sizeof(glm::vec3), &normals_obstacle[0], GL_STATIC_DRAW);
}
void setShaders(){
	// Get a handle for our "LightPosition" uniform
	LightID = glGetUniformLocation(programID_lighting, "LightPosition_worldspace");

	//Define vertex attributes
	vertexPosID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	vertexColorID = glGetUniformLocation(programID, "vertexColor");
	scalingFactorID = glGetUniformLocation(programID, "scalingFactor");
	vertexPosID_ligthing = glGetAttribLocation(programID_lighting, "vertexPosition_modelspace");
	vertexColorID_lighting = glGetUniformLocation(programID_lighting, "vertexColor");
	scalingFactorID_lighting = glGetUniformLocation(programID_lighting, "scalingFactor");
	vertexNormalID = glGetAttribLocation(programID_lighting, "vertexNormal_modelspace");
}
void initializeEverything(){
	initializeWindow();

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
	}

	// Ensure we can capture key presses
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

   	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	setObjectID();
	createBuffers();
	setShaders();
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	viewMatrix       = glm::lookAt(
								position,         // Camera is here
								glm::vec3(0,0,0), // and looks here : at the same position
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	glUseProgram(programID);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUseProgram(programID_lighting);
	glUniformMatrix4fv(ViewMatrixID_lighting, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(MatrixID_lighting, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
	// Set background colour
	glClearColor(0.53f, 0.8f, 0.92f, 1.0f);
}

void endEverything(){
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer_flap1);
	glDeleteBuffers(1, &elementbuffer_flap);
	glDeleteBuffers(1, &vertexbuffer_flap2);
	glDeleteBuffers(1, &vertexbuffer_obstacle);
	glDeleteBuffers(1, &elementbuffer_obstacle);
	glDeleteBuffers(1, &normalbuffer_obstacle);
	glDeleteProgram(programID);
	glDeleteProgram(programID_lighting);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	quit = true;
}
void display(GLuint vertex){
	// 1st attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosID);
		glBindBuffer(GL_ARRAY_BUFFER, vertex);
		glVertexAttribPointer(
			vertexPosID,        // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_flap);

		// // Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices_flap.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

		glDisableVertexAttribArray(vertexPosID);
}
void display_obstacle(){
	// 1st attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosID_ligthing);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_obstacle);
		glVertexAttribPointer(
			vertexPosID_ligthing,        // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// 2rd attribute buffer : normals
		glEnableVertexAttribArray(vertexNormalID);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_obstacle);
		glVertexAttribPointer(
			vertexNormalID,                   // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);


		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_obstacle);

		// // Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices_obstacle.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

		glDisableVertexAttribArray(vertexPosID_ligthing);
		glDisableVertexAttribArray(vertexNormalID);
}


void  mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	if(!allow && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		allow = true;
	}
	else if(allow && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ){
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if(xpos<1024&&xpos>0&&ypos>0&&ypos<768){
			glm::mat4 inverseMatrix = glm::inverse(projectionMatrix*viewMatrix*rotationMatrixFrame);
			float z = rand()%4+1;
			z -= 2;
			if(z > -0.00001 && z<0.00001)
				z = 1;
			float a = (xpos-512.0)/512.0, b = (384.0-ypos)/384.0;
			float c = glm::vec4(inverseMatrix*glm::vec4(a,b,0.0,1.0))[2]-glm::vec4(inverseMatrix*glm::vec4(a,b,0.0,1.0))[3]*z;
			c = c/(z*inverseMatrix[2][3]-inverseMatrix[2][2]);
			float x = z/glm::vec4(inverseMatrix*glm::vec4(a,b,c,1.0))[2];
			glm::vec4 posTemp = inverseMatrix*glm::vec4(a*x,b*x,c*x,x);
			posTemp = glm::vec4((posTemp[0]-xmin)/(-xmin+xmax),(posTemp[1]-ymin)/(-ymin+ymax),(posTemp[2]-zmin)/(-zmin+zmax),1);
			if(glfwGetKey( window, GLFW_KEY_B ) == GLFW_PRESS){
				sys.addBoid(glm::vec3(posTemp[0],posTemp[1],posTemp[2]));
				allow = false;
			}
			else if(glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS){
				sys.addPredator(glm::vec3(posTemp[0],posTemp[1],posTemp[2]));
				allow = false;
			}
			else if(glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS){
				sys.addObstacle(glm::vec3(posTemp[0],posTemp[1],posTemp[2]));
				allow = false;
			}
		}
	}
}
void output(std::vector<boid> &boids, std::vector<predator> &predators, std::vector<glm::vec3> &obstacles){
	//clear Everything on window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Compute the MVP matrix from keyboard and mouse input
	rotationMatrixFrame = computeRotationOfFrame();
	glUseProgram(programID);
	glUniform1f(scalingFactorID, 1.5);
	glUniform3f(vertexColorID, boidColor.x, boidColor.y, boidColor.z);
	for(int i=0;i<boids.size();++i){	
		glm::vec3 position = boids[i].getPosition();
		position = glm::vec3(position[0]*(xmax-xmin)+xmin, position[1]*(ymax-ymin)+ymin, position[2]*(zmax-zmin)+zmin);
		glm::vec3 currVelocity = boids[i].getVelocity();
		glm::vec3 prevVelocity = glm::vec3(0,1,0);
		glm::vec3 rotationAxis = glm::normalize(glm::cross(prevVelocity,currVelocity));
		int flapStatus = boids[i].updateFlapStatus();
		double rotationAngle = glm::angle(glm::normalize(currVelocity),glm::normalize(prevVelocity));

		glm::mat4 rotationMatrixBoid = glm::rotate(float(rotationAngle), rotationAxis);
		glm::mat4 translationMatrixBoid = glm::translate(glm::mat4(), position);
		glm::mat4 modelMatrix = rotationMatrixFrame*translationMatrixBoid*rotationMatrixBoid;

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		if(flapStatus < flapStates/2)
			display(vertexbuffer_flap1);
		else 
			display(vertexbuffer_flap2);
		if(glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0){
			endEverything();
			break;
		}
	}
	glUseProgram(programID);
	glUniform1f(scalingFactorID, 3.0);
	glUniform3f(vertexColorID, predatorColor.x, predatorColor.y, predatorColor.z);
	for(int i=0;i<predators.size();++i){	
		glm::vec3 position = predators[i].getPosition();
		position = glm::vec3(position[0]*(xmax-xmin)+xmin, position[1]*(ymax-ymin)+ymin, position[2]*(zmax-zmin)+zmin);
		glm::vec3 currVelocity = predators[i].getVelocity();
		glm::vec3 prevVelocity = glm::vec3(0,1,0);
		glm::vec3 rotationAxis = glm::normalize(glm::cross(prevVelocity,currVelocity));
		int flapStatus = predators[i].updateFlapStatus();
		double rotationAngle = glm::angle(glm::normalize(currVelocity),glm::normalize(prevVelocity));

		glm::mat4 rotationMatrixPredator = glm::rotate(float(rotationAngle), rotationAxis);
		glm::mat4 translationMatrixPredator = glm::translate(glm::mat4(), position);
		glm::mat4 modelMatrix = rotationMatrixFrame*translationMatrixPredator*rotationMatrixPredator;
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		if(flapStatus < flapStates/2)
			display(vertexbuffer_flap1);
		else 
			display(vertexbuffer_flap2);	
		if(glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0){
			endEverything();
			break;
		}
	}
	glUseProgram(programID_lighting);
	glUniform1f(scalingFactorID_lighting, 5.0);
	glUniform3f(vertexColorID_lighting, obstacleColor.x, obstacleColor.y, obstacleColor.z);
	for(int i=0;i<obstacles.size();++i){	
		glm::vec3 position = obstacles[i];
		position = glm::vec3(position[0]*(xmax-xmin)+xmin, position[1]*(ymax-ymin)+ymin, position[2]*(zmax-zmin)+zmin);
		glm::mat4 translationMatrixObstacle = glm::translate(glm::mat4(), position);
		glm::mat4 modelMatrix = rotationMatrixFrame*translationMatrixObstacle;
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(ModelMatrixID_lighting, 1, GL_FALSE, &modelMatrix[0][0]);
		display_obstacle();
		if(glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0){
			endEverything();
			break;
		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}