CC = g++
CXXFLAGS = -Wall -DGL_SILENCE_DEPRECATION -O3 -std=c++11
OPENGLLIBS = -framework OpenGL -framework GLUT -lpng

vector-fields: vector-fields.o class-OpenGL.o
	$(CC) $^ -o $@ $(OPENGLLIBS)

class-OpenGL.o: class-OpenGL.cc
	$(CC) -c $< $(CXXFLAGS)

clean:
	rm -f *.o vector-fields
