CC = g++
CXXFLAGS = -Wall -Wno-deprecated -DGL_SILENCE_DEPRECATION
OPENGLLIBS = -lglut -lGL -lGLU   # for Linux

OS := $(shell uname)
ifeq ($(OS),Darwin)
 OPENGLLIBS = -framework Carbon -framework OpenGL -framework GLUT  # for MacOS
endif


all: VectorField

VectorField: vector-fields.o
	$(CC) $^ -o $@ $(OPENGLLIBS)

vector-fields.o: vector-fields.cc
	$(CC) -c $< $(CXXFLAGS)

clean:
	rm -f VectorField vector-fields.o

