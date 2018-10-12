CXX = g++ -std=c++11
IDIR = ../include
ODIR = ./obj
CXXFlAG = -I$(IDIR)
_DEPS = shader.h params.h system.h boid.h predator.h
SRC = inout.cpp main.cpp system.cpp boid.cpp shader.cpp predator.cpp
OBJ = $(patsubst %,$(ODIR)/%,$(SRC:.cpp=.o))
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
LIBS = -lglfw -lglew -framework OpenGL
LIBS_linux = -lGL -lGLU -lglut -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c $< -o $@ $(CXXFlAG)

mac : $(OBJ)
	$(CXX) $^ -o app $(LIBS)
linux : $(OBJ)
	$(CXX) $^ -o app $(LIBS_linux)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o core