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

#include "class-OpenGL.h"


void (*OpenGL_Display::display_Function)(void) =        nullptr;
void (*OpenGL_Display::initialize_Function)(void) =     nullptr;
void (*OpenGL_Display::reshape_Function)(int w,int h) = nullptr;
void (*OpenGL_Display::keyboard_Function)(unsigned char key, int x, int y) = nullptr;
void (*OpenGL_Display::specialkeys_Function)(int key, int x, int y) = nullptr;
void (*OpenGL_Display::idle_Function)(void) = nullptr;
void (*OpenGL_Display::mouse_Function)(int button, int state, int x, int y) = nullptr;
void (*OpenGL_Display::mousemotion_Function)(int x, int y) = nullptr;




void OpenGL_Display::CallOpenGLMainFunctions(int argc, char** argv)
{
 glutInit(&argc, argv);                        // Carga la estructura de datos internas de GLUT.
 //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // Características de la Tarjeta Gráfica a usar.
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);


 glutInitWindowSize(1000,500);                 // Tamaño Ventana
 glutInitWindowPosition(150, 150);             // Posición (Esq. superior izquierda)
 glutCreateWindow("Ventana");                  // Ventana tipo OpenGL (ventana típica del sistema operativo)

 //glutInitWindowPosition(100,100);
 //glutInitWindowSize(640,360);
 //glutCreateWindow("SnowMen from 3D-Tech");



 
 InitializeScene(initialize_Function);          // Passing either nullptr or a function
 glutIdleFunc(IdleFunction);                    // This will call glutDisplayFunc repeteadly
 //glutDisplayFunc(display_Function);           // This function draws EVERYTHING 
 //glutDisplayFunc(DrawOneVertex);              // Dummy display 
 glutDisplayFunc(Draw);                         // This function draws EVERYTHING that  "Draw" indicates
 glutReshapeFunc(reshape_Function);             // Redefine the geometry of the scene
 glutKeyboardFunc(Keyboard);                    // Redefine keyboard function 
 glutSpecialFunc(SpecialKeys);
 glutMouseFunc(Mouse);                          // Redefine mouse function
 glutMotionFunc(MouseMotion);                   // Redefine motion function
 glutMainLoop();                                // Infinite loop over the above function 

}

void OpenGL_Display::InitializeScene(void (*init_func)() ){

 if (init_func != nullptr)  init_func();
 else{
  std::cout << "Default InitializeScene() from OpenGL_Display class" << std::endl;
  glClear(GL_COLOR_BUFFER_BIT); // Clear the screen  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // RGBA values (1.0f = 255)
  const double ancho=2640.0;
  const double alto=480.0;
  double xmax=ancho/2, ymax=alto/2, xmin=-ancho/2, ymin=-alto/2;
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
	glPointSize(1.0);
 }
}

void OpenGL_Display::EmptyWindow(void)
{
 glClear(GL_COLOR_BUFFER_BIT); // Clears the screen 
 glFlush();                    // Sends info to the screen (draw)
}

void OpenGL_Display::DrawOneVertex(void)
{
 glClear(GL_COLOR_BUFFER_BIT); // Clear the screen  
 glBegin(GL_POINTS);
   glColor3d(1,0,0);
   glVertex2d( 0, 0);
 	glPointSize(200);
 glEnd();
 //	glutPostRedisplay();  //  function is used to mark the current window as needing to be redisplayed. It essentially triggers a call to the display function (registered with glutDisplayFunc)
 //std::cout << "Swapping Buffers\n";
 glutSwapBuffers();  // Clears the screen for glutIdleFunc 
}

void OpenGL_Display::Draw()
{
 if (display_Function != nullptr)  { display_Function(); }
 //else                       { DrawOneVertex(); }
 else                       { EmptyWindow(); }
}

void OpenGL_Display::Keyboard(unsigned char key, int x, int y)
{
 if (keyboard_Function != nullptr)  keyboard_Function(key,x,y);
 else{
	switch (key){
		case 'q':
		case 27 : exit(0); break;

		case 'f': glutFullScreen();break;
		case 'F': glutReshapeWindow(600,420);
			  glutPositionWindow(320,320);break;
	}
 }
}

void OpenGL_Display::Mouse(int button, int state, int x, int y)
{
 if (mouse_Function != nullptr)  mouse_Function(button, state, x, y);
 else{
  //switch(button)
  //{
  // case GLUT_LEFT_BUTTON:   Buttons[0] = (state==GLUT_DOWN)?1:0;
  //  if (state==GLUT_UP)
  //  {
  //   RotUp=false; RotDown=false; RotLeft=false; RotRight=false; Zup=false; Zdown=false;
  //   (sseg->disp)->MakeZoomer(zoomID);
  //  }
  //  break;
  // case GLUT_MIDDLE_BUTTON:     Buttons[1] = (state==GLUT_DOWN)?1:0;    break;
  // case GLUT_RIGHT_BUTTON:      Buttons[2] = (state==GLUT_DOWN)?1:0;    break;
  // default:
  //  break;
  //}
 }
}

void OpenGL_Display::MouseMotion(int x, int y)
{
 if (mousemotion_Function != nullptr)  mousemotion_Function(x, y);
 else{
  //switch(button)
  //{
  // case GLUT_LEFT_BUTTON:   Buttons[0] = (state==GLUT_DOWN)?1:0;
  //  if (state==GLUT_UP)
  //  {
  //   RotUp=false; RotDown=false; RotLeft=false; RotRight=false; Zup=false; Zdown=false;
  //   (sseg->disp)->MakeZoomer(zoomID);
  //  }
  //  break;
  // case GLUT_MIDDLE_BUTTON:     Buttons[1] = (state==GLUT_DOWN)?1:0;    break;
  // case GLUT_RIGHT_BUTTON:      Buttons[2] = (state==GLUT_DOWN)?1:0;    break;
  // default:
  //  break;
  //}
 }
}
 
void OpenGL_Display::SpecialKeys(int key, int x, int y)
{
if (specialkeys_Function != nullptr)  specialkeys_Function(key,x,y);
else{
	switch (key) {
//		case GLUT_KEY_LEFT  : theta += 1.0;break;
//		case GLUT_KEY_RIGHT : theta -= 1.0;break;
//		case GLUT_KEY_UP    : rho  -= 1.0;break;
//		case GLUT_KEY_DOWN  : rho  += 1.0;break;
	}
 }
}

void OpenGL_Display::IdleFunction()
{
 if (idle_Function != nullptr)  { idle_Function(); }
 else                       { glutPostRedisplay(); }
}


void OpenGL_Display::SaveFramePNG(const std::string& filename, int width, int height, const std::vector<unsigned char>& pixels) {
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Error creating PNG write struct" << std::endl;
        fclose(file);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        std::cerr << "Error creating PNG info struct" << std::endl;
        png_destroy_write_struct(&png, nullptr);
        fclose(file);
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "Error during PNG creation" << std::endl;
        png_destroy_write_struct(&png, &info);
        fclose(file);
        return;
    }

    png_init_io(png, file);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    // Prepare row pointers
    std::vector<png_bytep> rowPointers(height);
    std::vector<unsigned char> mutablePixels(pixels.begin(), pixels.end());  // Create mutable copy of pixels
    for (int y = 0; y < height; ++y) {
        //rowPointers[y] = reinterpret_cast<png_bytep>(mutablePixels.data() + (y * width * 3));
        rowPointers[y] = reinterpret_cast<png_bytep>(const_cast<unsigned char*>(&pixels[(height - 1 - y) * width * 3]));
    }

    png_write_image(png, rowPointers.data());
    png_write_end(png, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(file);
}






