all: VectorField

LIBS= -lglut -lGL -lGLU

OS := $(shell uname)
ifeq ($(OS),Darwin)
 LIBS= -framework Carbon -framework OpenGL -framework GLUT
endif

VectorField: vector-fields.o
	g++ vector-fields.o  -o VectorField $(LIBS)

vector-fields.o: vector-fields.cc
	g++ -c vector-fields.cc

clean:
	rm *.o
