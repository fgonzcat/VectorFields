CC = g++
CXXFLAGS = -Wall -Wno-deprecated 
OPENGLLIBS = -lglut -lGL -lGLU -DGL_SILENCE_DEPRECATION  # for Linux

OS := $(shell uname)
ifeq ($(OS),Darwin)
 OPENGLLIBS = -framework Carbon -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION # for MacOS
endif


all: VectorField 

# $^ : Automatic variable representing all the prerequisites of the rule, in this case, vector-fields.o.
# -o $@: Specifies the output file name, which is VectorField.
VectorField: vector-fields.o
	$(CC) $^ -o $@ $(OPENGLLIBS)

# $< : represents the first prerequisite of the rule: vector-fields.cc
vector-fields.o: vector-fields.cc
	$(CC) -c $< $(CXXFLAGS)


# Alternative merge of linking libraries to .o
#VectorField: vector-fields.o
#	$(CC) vector-fields.cc -o VectorField $(CXXFLAGS) $(OPENGLLIBS)

clean:
	rm -f VectorField vector-fields.o

