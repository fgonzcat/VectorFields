//--------------------------------------------------//
// This program draws a vector field of the form    //
//                E(r) = E(r)e^{i(k*r-w*t)},        //
// where r=(x,y,z) and k=(kx,ky,kz)                 //
//--------------------------------------------------//

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
 #include <GL/glut.h>
#endif


#include <iostream>
#include <cmath>

#define ESCAPE 27

unsigned char Buttons[3] = {0};
int mousecoord[2];
int win_width=800, win_height=600;
float ancho=50.0, alto=50.0;
float diagonal=sqrt(ancho*ancho+alto*alto);
float dist=diagonal;
float fovy;
float near, far;
float eye[3], center[3], up[3];
float v[3], diag[3], camdir[3];
float tx = 0;
float ty = 0;
bool paused=false;
GLuint unitID, vecfieldID;

float dt=0.001;
float t=0;

int NumXArrows=50;
int NumYArrows=50;
int NumZArrows=1;
float norm=0.8;
float lambda=10;
float w=300;
float kx(float x, float y, float z){ return (2*M_PI/lambda)*x/sqrt(x*x+y*y);}
float ky(float x, float y, float z){ return (2*M_PI/lambda)*y/sqrt(x*x+y*y);}
float kz(float x, float y, float z){ return 0.0;}
float Ex(float x, float y, float z){ return 0.0; }
float Ey(float x, float y, float z){ return 0.0; }
float Ez(float x, float y, float z){ return 1.0; }


//------------------------------- SolidCylinder ------------------------------//
// Draw a cone of radii r and height h
void SolidCylinder(float r, float h)
{
 float vtx=4;
 glBegin(GL_POLYGON);
  for (int n=0; n<=vtx; n++) glVertex3d(r*cos(2*M_PI*n/vtx),r*sin(2*M_PI*n/vtx),0);
 glEnd();
 glBegin(GL_POLYGON);
  for (int n=0; n<vtx; n++)
  {
   glVertex3d(r*cos(2*M_PI*n/vtx),r*sin(2*M_PI*n/vtx),0);
   glVertex3d(r*cos(2*M_PI*(n+1)/vtx),r*sin(2*M_PI*(n+1)/vtx),0);
   glVertex3d(r*cos(2*M_PI*(n+1)/vtx),r*sin(2*M_PI*(n+1)/vtx),h);
   glVertex3d(r*cos(2*M_PI*n/vtx),r*sin(2*M_PI*n/vtx),h);
   glVertex3d(r*cos(2*M_PI*n/vtx),r*sin(2*M_PI*n/vtx),0);
  }
 glEnd();
}

//------------------------------- SolidCone ------------------------------//
// Draw a cone of radii r and height h
void SolidCone(float r, float h)
{
 float vtx=4;
 glBegin(GL_POLYGON);
  for (int n=0; n<=vtx; n++) glVertex3d(r*cos(2*M_PI*n/vtx),r*sin(2*M_PI*n/vtx),0);
 glEnd();
 glBegin(GL_POLYGON);
  for (int n=0; n<vtx; n++)
  {
   glVertex3d(r*cos(2*M_PI*n/vtx),r*sin(2*M_PI*n/vtx),0);
   glVertex3d(r*cos(2*M_PI*(n+1)/vtx),r*sin(2*M_PI*(n+1)/vtx),0);
   glVertex3d(0,0,h);
   glVertex3d(r*cos(2*M_PI*n/vtx),r*sin(2*M_PI*n/vtx),0);
  }
 glEnd();
}

//------------------------------ Unitary Vector ------------------------------//
void MakeUnitZ(GLuint id)
{
 float r=diagonal/1000;
 glNewList(id,GL_COMPILE);
 glPushMatrix();
  SolidCylinder(r,1);  // Cilindro
//  glBegin(GL_LINES); glVertex3f(0,0,0); glVertex3f(0,0,1);  glEnd(); // Linea simple
 glPopMatrix();
 glEndList();
}

//------------------------------ General Vector ------------------------------//
void flecha(float x, float y, float z)
{
 float L=x*x+y*y+z*z;
 L=sqrt(L);
 float r=diagonal/1000;
 glPushMatrix();
  float angle=acos(z/L)*180/M_PI;
  // Rotamos el cilindro alargado (0,0,L) un angulo
  // (-angle) en torno al eje (x,y,z)X(0,0,L)=(L*y,-L*x,0)=(ejex,ejey,ejez)
  float ejex=L*y, ejey=-L*x, ejez=0;
  if (fabs(L*y)<0.001 && fabs(L*x)<0.001) ejex=1; //Soluciona el caso eje=(0,0,0)
  glRotated(-angle, ejex, ejey, ejez);
  glPushMatrix();
  // 'Cilindro'
  glPushMatrix();
   glScalef(1,1,L); glCallList(unitID); // Cilindro agrandado
  glPopMatrix();
  // 'Gorrito'
  glTranslated(0,0,L);
  SolidCone(2*r,0.1*L);
//  glutSolidCone(2*r,4*r,4,4);
  glPopMatrix();
 glPopMatrix();
}

//****************************************************************************//
//------------------------------ Vector Field --------------------------------//
//****************************************************************************//
void MakeVectorField(GLuint id)
{
 glNewList(id,GL_COMPILE);
 int counter=0;
 for (float z=0; z<ancho/4; z+=(ancho/4)/NumZArrows)
 {
 for (float y=-alto/2; y<alto/2; y+=alto/NumYArrows)
 {
  for (float x=-ancho/2; x<ancho/2; x+=ancho/NumXArrows)
  {
   glPushMatrix();
   glTranslatef(x,y,z);
   glColor3f(1,1,0);
   if (Ez(x,y,z)*cos(kx(x,y,z)*x+ky(x,y,z)*y+kz(x,y,z)*z-w*t)<0) glColor3f(1,1,0);
   else    glColor3f(1,0,0);
   counter++;
   flecha(
    Ex(x,y,z)*cos(kx(x,y,z)*x+ky(x,y,z)*y+kz(x,y,z)*z-w*t)/norm,
     Ey(x,y,z)*cos(kx(x,y,z)*x+ky(x,y,z)*y+kz(x,y,z)*z-w*t)/norm,
      Ez(x,y,z)*cos(kx(x,y,z)*x+ky(x,y,z)*y+kz(x,y,z)*z-w*t)/norm);
//   glColor3f(0,0,1);
//   flecha(kx(x,y,z),ky(x,y,z),kz(x,y,z));
   glPopMatrix();
  }
 }
 }
 glEndList();
// std::cout << "Conte " << counter << " flechas\n";
}
//----------------------------------------------------------------------------//


//--------------------------- RotateVector -----------------------------------//
// It is used to rotate the camera in the "MoveCamera" function
// Method taken from http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/
void RotateVector(float * vect, float * axis, float ang)
{
 float u=axis[0],v=axis[1],w=axis[2];
 float x=vect[0],y=vect[1],z=vect[2];
 float norm2=u*u+v*v+w*w;

 float rv[3];
 rv[0] = u*(u*x+v*y+w*z)+(x*(v*v+w*w)-u*(v*y+w*z))*cos(ang)+sqrt(u*u+v*v+w*w)*(-w*y+v*z)*sin(ang);
 rv[1] = v*(u*x+v*y+w*z)+(y*(u*u+w*w)-v*(u*x+w*z))*cos(ang)+sqrt(u*u+v*v+w*w)*(w*x-u*z)*sin(ang);
 rv[2] = w*(u*x+v*y+w*z)+(z*(u*u+v*v)-w*(u*x+v*y))*cos(ang)+sqrt(u*u+v*v+w*w)*(-v*x+u*y)*sin(ang);

 for (int q=0; q<3; ++q) vect[q]=rv[q]/norm2;

}
//------------------------------------- SetCamera ----------------------------//
// This function will be used in any function that involves a camera movement
void SetCamera(float distance)
{
 for (int q=0;q<3;++q) center[q] = 0+tx*v[q]+ty*up[q];//0.5*diag[q] + tx*v[q] + ty*up[q];
 for (int q=0;q<3;++q) eye[q] = center[q] + distance*camdir[q];
 far=4*fabs(distance);
 near=0.5*fabs(distance);
}

void SetCamera(float theta, float phi)
{
 // 'camdir' is the unitary vector that points from the center of the cell towards the camera
 camdir[0] = sin(theta)*cos(phi);
 camdir[1] = sin(theta)*sin(phi);
 camdir[2] = cos(theta);
 float phz = 0.5*M_PI;
 // 'v' is a 90° rotation of the vector 'camdir' (adding 0.5*M_PI to the "theta" angle in spherical coordinates) along the plane phi=const.
 v[0] = sin(theta+phz)*cos(phi);
 v[1] = sin(theta+phz)*sin(phi);
 v[2] = cos(theta+phz);
 // 'up'='camdir'x'v' vector indicates which direction is up (the direction from the bottom to the top of the viewing volume)
 up[0] = camdir[1]*v[2] - camdir[2]*v[1];
 up[1] = camdir[2]*v[0] - camdir[0]*v[2];
 up[2] = camdir[0]*v[1] - camdir[1]*v[0];

  // Set the far-clip (perspetive mode) always beyond the objects
 far=4*fabs(dist);
 SetCamera(dist);
}

//------------------------------- MoveCamera ---------------------------------//
void MoveCamera(float angx, float angy)
{
 float rotx = M_PI*angx/180.0;
 float roty = M_PI*angy/180.0;

 float horiz[3];
 // 'horiz'='up'x'camdir'='v'
 horiz[0] = up[1]*camdir[2] - up[2]*camdir[1];
 horiz[1] = up[2]*camdir[0] - up[0]*camdir[2];
 horiz[2] = up[0]*camdir[1] - up[1]*camdir[0];

 RotateVector(camdir, horiz, roty);
 RotateVector(camdir, up, rotx);

 RotateVector(v, horiz, roty);
 RotateVector(v, up, rotx);
 
 // 'up'='camdir'x'v'
 up[0] = camdir[1]*v[2] - camdir[2]*v[1];
 up[1] = camdir[2]*v[0] - camdir[0]*v[2];
 up[2] = camdir[0]*v[1] - camdir[1]*v[0];

 SetCamera(dist);
}

//----------------------------- Init -----------------------------------------//
void Init(int argc, char** argv)
{
 GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
 GLfloat light_position[] = { 0, 0, -1, 0};
 GLfloat light_position1[] = {0, 0, 1, 0};
  
 GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
 GLfloat mat_shininess[] = { 50.0 };
 glShadeModel (GL_SMOOTH);
 
 /* Define normal light */
 glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
 glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse);
 glLightfv(GL_LIGHT0,GL_POSITION,light_position);
 glLightfv(GL_LIGHT1,GL_POSITION,light_position1);

 /* Define material of objects */
 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

 /* Enable a single OpenGL light */
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHT1);

 /* Use depth buffering for hidden surface elimination */
 glEnable(GL_DEPTH_TEST);
 
 /* Enable the color material mode */
 glEnable(GL_COLOR_MATERIAL);
 
 /* Don't mess up the lighting equations */
 glEnable(GL_NORMALIZE);

 near=0.5*diagonal;
 far=2*diagonal;
 fovy = (GLfloat)( 0.5*alto/(2*near) );
 fovy = (GLfloat)( 2*atan((float)fovy)/M_PI*180.0 ); // Field of view touches exactly the borders of the cell
 fovy = (GLfloat)( 1.2*fovy); // We open the field of view a little bit more.
 
 for (int q=0; q<3; q++) diag[q]=0.5*(alto+ancho);
 double theta=0, phi=0;
 camdir[0] = sin(theta)*cos(phi);
 camdir[1] = sin(theta)*sin(phi);
 camdir[2] = cos(theta);
 float phz = 0.5*M_PI;
 /* Set 'v', a 90° rotation of the vector 'camdir' (adding 0.5*M_PI to the "theta" angle in spherical coordinates) along the plane phi=const. */
 v[0] = sin(theta+phz)*cos(phi); 
 v[1] = sin(theta+phz)*sin(phi);
 v[2] = cos(theta+phz);
 /* Set 'up'='camdir'x'v' vector, that indicates which direction is up (the direction from the bottom to the top of the viewing volume) */
 up[0] = camdir[1]*v[2] - camdir[2]*v[1];
 up[1] = camdir[2]*v[0] - camdir[0]*v[2];
 up[2] = camdir[0]*v[1] - camdir[1]*v[0];
 float cm = 0.0;
 for (int q=0;q<3;++q) cm += up[q]*up[q];
 for (int q=0;q<3;++q) up[q] /= sqrt(cm);
 

 /* SetCamera(distance) or SetCamera(orth_distance) */
 MoveCamera(0,20);
 
 /* Make the a unitary vector un z direction */
 unitID = glGenLists(1);
 MakeUnitZ(unitID);
 
 /* Make the whole vector field */
 vecfieldID = glGenLists(1);
 MakeVectorField(vecfieldID);
 
}

//----------------------------- Reshape ---------------------------------------//
void Reshape(int w, int h)
{
  win_width=w;
  win_height=h;
  // set the GL viewport to match the full size of the window
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  float aspect = w/(float)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // we math the aspect ratio (w/h) with the viewport ratio in both perspectives
  gluPerspective(fovy,aspect,near,far);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

//----------------------------- Mouse ---------------------------------------//
void Mouse(int button,int state,int x,int y){

 mousecoord[0] = x;
 mousecoord[1] = y;

 switch(button)
 {
  case GLUT_LEFT_BUTTON:
   Buttons[0] = ((GLUT_DOWN==state)?1:0);
   break;
  case GLUT_MIDDLE_BUTTON:
   Buttons[1] = ((GLUT_DOWN==state)?1:0);
   break;
  case GLUT_RIGHT_BUTTON:
   Buttons[2] = ((GLUT_DOWN==state)?1:0);
   break;
  default:
   break;		
 }
 mousecoord[0] = x;
 mousecoord[1] = y;
 
 glutPostRedisplay();
}

//----------------------------- Mouse Motion ---------------------------------//
void MouseMotion(int x,int y){

 // drag and drop
 int dx = x-mousecoord[0];
 int dy = y-mousecoord[1];
 mousecoord[0] = x;
 mousecoord[1] = y;
 
 if( Buttons[1] )            // Boton del centro (scroll)
 {
  dist -= (float) 1.1f * dx;
  MoveCamera(0,0);
 }
 else if( Buttons[0] )      // Boton izquierdo
 {
  if (sqrt(dx*dx+dy*dy) < 0.2) return;
  MoveCamera(-0.5*dx,-0.5*dy);
 }
 else if( Buttons[2] )     // Boton derecho
 {
  tx -= (float) 0.1f * dx;
  ty += (float) 0.1f * dy;
  MoveCamera(0,0);
 }
 Reshape(win_width,win_height);
 glutPostRedisplay();
}

//----------------------------- Keyboard -------------------------------------//
void Keyboard(unsigned char key, int x, int y){
 switch (key)
 {
  case ESCAPE: exit(0);
  case 'q': exit(0);
   break;
  case 'a':
  {
   tx=0; ty=0;
   dist=1.1*alto/(2*tan(M_PI/8));
   SetCamera(0,0);
   glutPostRedisplay();
  }
   break;
  case 'p':
  case ' ':
  {
   paused = !paused;
  }
 }
}

//----------------------------- Draw -----------------------------------------//
void Draw(void){
// glClearColor(1,1,1,1);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();

 glPushMatrix();
 gluLookAt(
 (GLfloat)eye[0],(GLfloat)eye[1],(GLfloat)eye[2],
 (GLfloat)center[0],(GLfloat)center[1],(GLfloat)center[2],
 (GLfloat)up[0],(GLfloat)up[1],(GLfloat)up[2]);

 glPushMatrix();
  glCallList(vecfieldID);
  glColor3f(0,1,0);
  glBegin(GL_LINES);
   for(float i=-ancho/2;i<=ancho/2;i+=5){	glVertex3f(i,-alto/2,0);	glVertex3f(i,alto/2,0);}
   for(float i=-alto/2; i<=alto/2; i+=5){	glVertex3f(ancho/2,i,0);	glVertex3f(-ancho/2,i,0);}
  glEnd();
  glColor3f(1,1,1);
 glPopMatrix();

 glPushMatrix();
  glScaled(5,5,5);
  flecha(1,0,0);
  flecha(0,1,0);
  flecha(0,0,1);
  glPopMatrix();
 glPopMatrix();
 
 glutPostRedisplay();
 glutSwapBuffers();				// Intercambia los buffers para impedir el parpadeo.
}

void idle(void)
{
 if(!paused){t+=dt; MakeVectorField(vecfieldID);}
}

int main(int argc, char** argv)
{
 for (int op=0; op<argc; op++)
 {
  std::string sw=argv[op];
  if (sw=="-h")
  {
   std::cout << "-> INFORMATION:                                            \n";
   std::cout << "   This program can animate any vector field with the form \n";
   std::cout << "                    E(r)=E0(r)cos(kr-wt)                   \n";
   std::cout << "   where r=(x,y,z), k=(kx,ky,kz) and E0=(E0x,E0y,E0z).     \n";
   std::cout << "   Each component of k and E0 can depend on space and time,\n";
   std::cout << "   writting them in cartesian coordinates.                 \n";
   std::cout << "   You have the freedom to distribute copies of this free  \n";
   std::cout << "   software (and change it if you wish). For more information\n";
   std::cout << "   contact me to fgonzalez@lpmd.cl                       \n\n";
   std::cout << "-> SYNOPSIS:                                               \n";
   std::cout << "   Replace the piece of code necessary to configure your   \n";
   std::cout << "   vector field (see examples) and compile the program with\n\n";
   std::cout << "      g++ -Wall -o newfield vector-fields.cc -lglut -lGLU\n\n";
   std::cout << "   'a'                : Restores visualization.            \n";
   std::cout << "   'q'                : Closes program.                    \n";
   std::cout << "   'p' or ' '         : Play/stops the animation.          \n";
   std::cout << "   MOUSE_LEFT_BUTTON  : Rotate de scene.                   \n";
   std::cout << "   MOUSE_MIDDLE_BUTTON: Zoom to the scene.                 \n";
   std::cout << "   MOUSE_RIGHT_BUTTON : Translate the scene.             \n\n";
   std::cout << "-> INSTALL REQUIRED SOFTWARE:                              \n";
   std::cout << "   apt-get install mesa-utils                              \n";
   std::cout << "   apt-get install libglut3-dev                          \n\n";
   std::cout << "-> EXAMPLE:                                                \n";
   std::cout << "   This piece of code corresponds to a plane wave polarized\n";
   std::cout << "   in Z-direction, travelling in the direction (1,1,0):  \n\n";
   std::cout << "   int NumXArrows=50;                                      \n";
   std::cout << "   int NumYArrows=50;                                      \n";
   std::cout << "   int NumZArrows=1;                                       \n";
   std::cout << "   float norm=0.8;                                         \n";
   std::cout << "   float lambda=10;                                        \n";
   std::cout << "   float w=300;                                            \n";
   std::cout << "   float kx(float x, float y, float z){ return (2*M_PI/lambda)/sqrt(2);}\n";
   std::cout << "   float ky(float x, float y, float z){ return (2*M_PI/lambda)/sqrt(2);}\n";
   std::cout << "   float kz(float x, float y, float z){ return 0;}         \n";
   std::cout << "   float Ex(float x, float y, float z){ return 0; }     \n";
   std::cout << "   float Ey(float x, float y, float z){ return 0; }     \n";
   std::cout << "   float Ez(float x, float y, float z){ return 1; }   \n\n";
   std::cout << "   The first 3 lines fix the number of arrows in each      \n";
   std::cout << "   direction. In this case, you will have 50x50x1=2500     \n";
   std::cout << "   arrows in the simulation. 'norm' is a normalizer factor \n";
   std::cout << "   to keep arrows not so large, not so short. 'lambda' is  \n";
   std::cout << "   the wave length, and 'w' the frequency. 'kx', ky' and 'kz'\n";
   std::cout << "   are the componentes of the wave vector k, while 'Ex', 'Ey'\n";
   std::cout << "   and 'Ez' are the componentes of E0.                 \n\n\n";
   std::cout << "Felipe Gonzalez Cataldo.                                   \n";
   std::cout << "July 2011.                                                 \n";
   exit(0);
  }
 }
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 glutInitWindowSize(win_width,win_height);		
 glutInitWindowPosition(0, 150);
 glutCreateWindow("Campos Vectoriales");	

 Init(argc,argv);
 glutDisplayFunc(Draw);		
 glutKeyboardFunc(Keyboard);	
 glutReshapeFunc(Reshape);
 glutMotionFunc(MouseMotion);
 glutMouseFunc(Mouse);
 glutIdleFunc(idle);

 glutMainLoop();
 return 0;
}
