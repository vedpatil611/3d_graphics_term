SRC = cube.cpp sphere.cpp
OBJ = $(SRC:.cpp=.o)

all: cube sphere

.cpp.o:
	g++ -c -Wall -O3 $<

cube: cube.o
	g++ cube.o -o cube

sphere: sphere.o
	g++ sphere.o -o sphere

clean:
	rm -f cube sphere $(OBJ)

