SRC = cube.cpp sphere.cpp torus.cpp pyramid.cpp
OBJ = $(SRC:.cpp=.o)

all: cube sphere torus pyramid

.cpp.o:
	g++ -c -Wall -O3 $<

cube: cube.o
	g++ cube.o -o cube

sphere: sphere.o
	g++ sphere.o -o sphere

torus: torus.o
	g++ torus.o -o torus

pyramid: pyramid.o
	g++ pyramid.o -o pyramid
	
clean:
	rm -f cube sphere torus pyramid $(OBJ)

