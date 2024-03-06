CC = g++
CXXFLAGS = -Wall -O3 -std=c++11  -DGL_SILENCE_DEPRECATION
OPENGLLIBS= -lglut -lGL -lGLU

OS := $(shell uname)
ifeq ($(OS),Darwin)
 OPENGLLIBS = -framework Carbon -framework OpenGL -framework GLUT -lpng 
endif

vector-fields: vector-fields.o class-OpenGL.o
	$(CC) $^ -o $@ $(OPENGLLIBS)

class-OpenGL.o: class-OpenGL.cc
	$(CC) -c $< $(CXXFLAGS)

clean:
	rm -f *.o vector-fields
