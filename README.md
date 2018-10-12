# Starling Murmurations
The software structure is designed to simulate the motion of Starlings using C++. OpenGL is used for animation.

## Getting Started


### Prerequisites

To install glew:

```
cd Code
cd external/glew-1.13.0
make
sudo -s
make install
make clean
```

To install openGL:

```
cd Code
cd external/glfw-3.1.2
mkdir build
cd build
cmake ..
make
sudo make install
```

To install glm:

```
cd Code
cd external/glm-0.9.7.1
mkdir build
cd build
cmake ..
make
sudo make install
```


### Installing
 For Mac type:

```
cd Code/src
make clean
make mac
```

For Linux type:

```
cd Code/src
make clean
make linux
```

## Usage

To start the program type

```
./app <number of starlings to start with (Default 500)>
```

To add more boids to the screen, point at the desired position on the window and left click while pressing "B" on Keyboard.
To add predators to the screen, point at the desired position on the window and left click while pressing "P" on Keyboard.
To add still obstacles to the screen, point at the desired position on the window and left click while pressing "O" on Keyboard.

For rotating the frame of view, use the arrow keys.

## Authors

+ Garvit Gupta
**guptagarvit1997@gmail.com**

+ Madhur Singal
**madhursingal08@gmail.com**

*Course Project under [**Prof. Subhashis Banerjee**](http://http://www.cse.iitd.ernet.in/~suban/)*


