/***********************************************************************************************
-----------------------------------------------------------------------------------------------|
                             OPENGL CLASS                                                      |
                                                                                               |
This is a class that defines the object OpenGL_Display in order to be used in any other        |
code that wants to plot stuff on an OpenGL window.                                             |
                                                                                               |
Felipe Gonzalez                                                          Berkeley, 05/23/2023  |
-----------------------------------------------------------------------------------------------|
Compile on MacOS using
g++ -Wall -o example class-OpenGL.cc  -framework OpenGL -framework GLUT -Wno-deprecated
***********************************************************************************************/

#ifndef __CLASS_OPENGL_H__
#define __CLASS_OPENGL_H__


#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
 #include <GL/glut.h>
#endif

#include <string>
#include <vector>  // Include the vector header
#include <math.h>
#include<iostream>
#include <png.h>


using namespace std;

class OpenGL_Display
{
 private:
 static OpenGL_Display* instance; // Static instance of the class
 static void (*display_Function)(); // Pointer to the draw function
 static void (*reshape_Function)(int w ,int h); // Pointer to the Reshape function
 static void (*keyboard_Function)(unsigned char key, int x, int y); // Pointer to the Keyboard function
 static void (*specialkeys_Function)(int key, int x, int y); // Pointer to the SpecialKeys function
 static void (*mouse_Function)(int button, int state, int x, int y); // Pointer to the Mouse function
 static void (*mousemotion_Function)(int x, int y); // Pointer to the Motion function
 static void (*initialize_Function)();   // Pointer to the Initialize function
 static void (*idle_Function)();   // Pointer to the idle function
 int argc;
 char** argv; 


 public:
 OpenGL_Display(void){}
 OpenGL_Display(int Narg, char** options): argc(Narg), argv(options)  {} 
 ~OpenGL_Display(void){}

 // With static, I can call these functions below without creating an OpenGL_Display object.
 // ---> OpenGL_Display::CallOpenMainFunctions(argc,argv);
 // instead of 
 // ---> OpenGL_Display window1;  window1.CallOpenMainFunctions(argc,argv)
 //******** All these functions are part of the default creation a basic OpenGL display ******//
 static void CallOpenGLMainFunctions(int argc, char** argv);
 static void InitializeScene(void (*init_func)() );
 static void DrawOneVertex(void);
 static void EmptyWindow(void);
 static void Draw(void);
 static void Keyboard(unsigned char key, int x, int y);
 static void Mouse(int button, int state, int x, int y);
 static void MouseMotion(int x, int y);
 static void SpecialKeys(int key, int x, int y);
 static void IdleFunction(void);
 static void SaveFramePNG(const std::string& filename, int width, int height, const std::vector<unsigned char>& pixels);
 //static void Reshape(int w, int h);  // Not implemented as a general class module. Must be implemented by another class


 //******** The Set functions below are the optional functions the user must set ******//
 //  If these functions are called before CallOpenGLMainFunctions(), they will replace  the default calls.
 //  For instance,  void MyDrawing(void){ ... };   goes in int main() as   OpenGL_Display::SetDrawFunction(MyDrawing);
 //  
 //  ===========================================
 //  int main(){
 //   OpenGL_Diplay::SetInitializeFunction(InitializeScene);
 //   OpenGL_Display::SetDrawFunction(Dibuja);
 //   OpenGL_Display::SetReshapeFunction(Reshape);
 //   ...
 //   OpenGL_Display::CallOpenGLMainFunctions(argc,argv);
 //   return 0;
 //   }
 //  ===========================================
 //  
 //  After calling the Set functions, the respective pointers (set to nullptr) are no longer nullptr.

 static void SetDrawFunction(void (*draw_Func)()) { display_Function = draw_Func; }
 static void SetReshapeFunction(void (*reshp_Func)(int,int)) { reshape_Function = reshp_Func; }
 static void SetKeyboardFunction(void (*keyb_Func)(unsigned char key, int x, int y)) { keyboard_Function = keyb_Func; }
 static void SetSpecialKeysFunction(void (*speckey_Func)(int key, int x, int y)) { specialkeys_Function = speckey_Func; }
 static void SetInitializeFunction(void (*init_Func)()) { initialize_Function = init_Func; }
 static void SetMouseFunction(void (*mouse_Func)(int button, int state, int x, int y)) { mouse_Function = mouse_Func; }
 static void SetMouseMotionFunction(void (*mousemotion_Func)(int x, int y)) { mousemotion_Function = mousemotion_Func; }
 static void SetIdleFunction(void (*id_Func)()) { idle_Function = id_Func; }

};


#endif
