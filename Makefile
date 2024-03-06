CC = g++
CXXFLAGS = -Wall -O3 -std=c++11 -Wno-deprecated-declarations  
OPENGLLIBS= -lglut -lGL -lGLU

OS := $(shell uname)
ifeq ($(OS),Darwin)
 OPENGLLIBS = -framework Carbon -framework OpenGL -framework GLUT -lpng -DGL_SILENCE_DEPRECATION
endif

# $^ : Automatic variable representing all the prerequisites of the rule, in this case, vector-fields.o.
# -o $@: Specifies the output file name, which is VectorField.
vector-fields: vector-fields.o class-OpenGL.o
	$(CC) $^ -o $@ $(OPENGLLIBS)


# $< : represents the first prerequisite of the rule: vector-fields.cc
class-OpenGL.o: class-OpenGL.cc
	$(CC) -c $< $(CXXFLAGS)


# Alternative merge of linking libraries to .o
#VectorField: vector-fields.o
#	$(CC) vector-fields.cc -o VectorField $(CXXFLAGS) $(OPENGLLIBS)


clean:
	rm -f *.o vector-fields
