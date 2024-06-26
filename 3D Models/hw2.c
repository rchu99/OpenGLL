 #include <stdio.h>
 #include <stdlib.h>
 #include <stdarg.h>
 #include <time.h>
 #include <math.h>
 //  OpenGL with prototypes for glext
 #define GL_GLEXT_PROTOTYPES
 #ifdef __APPLE__
 #include <GLUT/glut.h>
 #else
 #include <GL/glut.h>
 #endif

 int th=0;         
 int ph=0;         
 int mode=0;       
 double asp=1;     
 double dim=6;   


 #define Cos(th) cos(3.1415927/180*(th))
 #define Sin(th) sin(3.1415927/180*(th))
 /*
  *  Convenience routine to output raster text
  *  Use VARARGS to make this more flexible
  */
 #define LEN 8192  //  Maximum length of text string
 void Print(const char* format , ...)
 {
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
 }

 /*
  *  Set projection
  */
 static void projection()
 {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (mode == 1 || mode == 2)
       gluPerspective(60,asp,dim/4,4*dim);
    else
       glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 }
 
  /*
  Car at position x,y,z and scaled with dx,dy,dz
  */
 static void car(double x,double y,double z,
                  double dx,double dy,double dz) {
	 
	glPushMatrix();
	glTranslated(x,y,z);
    glScaled(dx,dy,dz);
	
	glBegin(GL_QUADS);
	// Draw the car body
    glColor3f(1.0, 0.0, 0.0); // Red color
	
    
    // Front face
    glVertex3f(-1.0, -0.5, 1);
    glVertex3f(1.0, -0.5, 1);
    glVertex3f(1.0, 0.5, 1);
    glVertex3f(-1.0, 0.5, 1);
    
	glColor3f(1.0, 1.0, 0.0); // Yellow
    // Back face
    glVertex3f(-1.0, -0.5, -1.0);
    glVertex3f(1.0, -0.5, -1.0);
    glVertex3f(1.0, 0.5, -1.0);
    glVertex3f(-1.0, 0.5, -1.0);
    
	glColor3f(0.0, 1.0, 0.0);  // Green color
    // Top face
    glVertex3f(-1.0, 0.5, -1.0);
    glVertex3f(1.0, 0.5, -1.0);
    glVertex3f(1.0, 0.5, 1.0);
    glVertex3f(-1.0, 0.5, 1.0);
    
	glColor3f(1.0, 0.0, 1.0);  // Magenta color
    // Bottom face
    glVertex3f(-1.0, -0.5, -1.0);
    glVertex3f(1.0, -0.5, -1.0);
    glVertex3f(1.0, -0.5, 1.0);
    glVertex3f(-1.0, -0.5, 1.0);
    
	glColor3f(0.0, 1.0, 1.0);  // Cyan color
    // Right face
    glVertex3f(1.0, -0.5, -1.0);
    glVertex3f(1.0, 0.5, -1.0);
    glVertex3f(1.0, 0.5, 1.0);
    glVertex3f(1.0, -0.5, 1.0);
    
	glColor3f(0.0, 1.0, 0.0);  // Green color
    // Left face
    glVertex3f(-1.0, -0.5, -1.0);
    glVertex3f(-1.0, 0.5, -1.0);
    glVertex3f(-1.0, 0.5, 1.0);
    glVertex3f(-1.0, -0.5, 1.0);
	
	// Front hood face
	glColor3f(1.0, 0.0, 1.0);  // Magenta color
	glVertex3f(-2, -0.5, 1);
    glVertex3f(-1.5, 0.5, 1);
    glVertex3f(-1.0, 0.5, 1);
    glVertex3f(-1.0, -0.5, 1);
	
	glColor3f(1.0, 0.0, 0.0); // Red color
	// back hood face
	glVertex3f(-2, -0.5, -1);
    glVertex3f(-1.5, 0.5, -1);
    glVertex3f(-1.0, 0.5, -1);
    glVertex3f(-1.0, -0.5, -1);
	
	glColor3f(1.0, 1.0, 0.0); // Yellow
	// top hood face
	glVertex3f(-1.5, 0.5, 1.0);
	glVertex3f(-1.5, 0.5, -1.0);
	glVertex3f(-1.0, 0.5, -1.0);
	glVertex3f(-1.0, 0.5, 1.0);
	// bottom hood face
	glColor3f(0.0, 1.0, 1.0);  // Cyan color
	glVertex3f(-2, -0.5, 1.0);  // Bottom left
    glVertex3f(-2, -0.5, -1.0); // Bottom right
	glVertex3f(-1.0, -0.5, -1.0); // Top right
	glVertex3f(-1.0, -0.5, 1.0);  // Top left
	
	// left hood face
	glColor3f(0.0, 0, 1.0);  // Blue color
	glVertex3f(-2, -0.5, 1);
	glVertex3f(-2, -0.5, -1);
	glVertex3f(-1.5, 0.5, -1.0);
	glVertex3f(-1.5, 0.5, 1.0);
	
    glEnd();  // End drawing quads
	glPopMatrix();
	
 }
 
 /*
  House at position x,y,z and scaled with dx,dy,dz
  */
 static void house(double x,double y,double z,
                  double dx,double dy,double dz)
 {

   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   glBegin(GL_QUADS);
   //  Front wall
   glColor3f(0.0, 1.0, 0.0);  // Green color
   glVertex3f(-1,-1, 1);
   glVertex3f(1,-1, 1);
   glVertex3f(1,1, 1);
   glVertex3f(-1,1, 1);
   //  Back wall
   glColor3f(1.0, 1.0, 0.0); // Yellow
   glVertex3f(1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,1,-1);
   glVertex3f(1,1,-1);
   //  Right wall
   glColor3f(1.0, 0.0, 0.0); // Red color
   glVertex3f(1,-1,1);
   glVertex3f(1,-1,-1);
   glVertex3f(1,1,-1);
   glVertex3f(1,1,1);
   //  Left wall
   glColor3f(0.0, 1.0, 1.0);  // Cyan color
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,1);
   glVertex3f(-1,1,1);
   glVertex3f(-1,1,-1);
   //  Right roof
   glColor3f(1.0, 0.0, 1.0);  // Magenta color
   glVertex3f(1,0.5,1);
   glVertex3f(1,0.5,-1);
   glVertex3f(0,2,-1);
   glVertex3f(0,2,1);
   //  Left roof
   glColor3f(0.0, 0.0, 1.0);  // Blue color
   glVertex3f(-1,0.5,-1);
   glVertex3f(-1,0.5,1);
   glVertex3f(0,2,1);
   glVertex3f(0,2,-1);
   //  bottom cover
   glColor3f(1.0, 1.0, 1.0);  // White color
   glVertex3f(-1,-1,-1);
   glVertex3f(1,-1,-1);
   glVertex3f(1,-1,1);
   glVertex3f(-1,-1,1);
   // Chimney front
   glColor3f(1.0, 0.0, 0.0); // Red color
   glVertex3f(0.75,1, -0.25);
   glVertex3f(1,1, -0.25);
   glVertex3f(1,1.5,-0.25);
   glVertex3f(0.75,1.5, -0.25);
   // Chimney right
   glColor3f(1.0, 1.0, 0.0); // Yellow
   glVertex3f(1,1, -0.25);
   glVertex3f(1,1, -0.5);
   glVertex3f(1,1.5,-0.5);
   glVertex3f(1,1.5, -0.25);
   // Chimney rear
   glColor3f(0.0, 0.0, 1.0);  // Blue color
   glVertex3f(1,1, -0.5);
   glVertex3f(0.75,1, -0.5);
   glVertex3f(0.75,1.5, -0.5);
   glVertex3f(1,1.5,-0.5);
   // Chimney left
   glColor3f(1.0, 0.0, 1.0);  // Magenta color
   glVertex3f(0.75,1, -0.5);
   glVertex3f(0.75,1, -0.25);
   glVertex3f(0.75,1.5, -0.25);
   glVertex3f(0.75,1.5, -0.5);
   // Chimney top
   glColor3f(0.0, 1.0, 1.0);  // Cyan color
   glVertex3f(0.75,1.5, -0.25);
   glVertex3f(1,1.5,-0.25);
   glVertex3f(1,1.5,-0.5);
   glVertex3f(0.75,1.5, -0.5);

   glEnd();
   glBegin(GL_TRIANGLES);
   // Front roof wall
   glColor3f(0.0, 1.0, 0.0);  // Green color
   glVertex3f(1,1, 1);
   glVertex3f(0,2, 1);
   glVertex3f(-1,1, 1);
   // Back roof wall
   glColor3f(1.0, 1.0, 1.0);  // White color
   glVertex3f(-1,1,-1);
   glVertex3f(0, 2, -1);
   glVertex3f(1,1,-1);

   glEnd();
   glPopMatrix();
 }

 
 /*
  *  OpenGL (GLUT) calls this routine to display the scene
  */
 void display()
 {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    double Ex = 0, Ey = 0, Ez = 10, Cx = 0, Cy = 0, Cz = 0;

    if (mode == 0) // Orthogonal view
    {
        glRotatef(ph, 1, 0, 0);
        glRotatef(th, 0, 1, 0);
    }
    else if (mode == 1) // Perspective view
    {
        Ex = -2 * dim * sin(th * 3.1415927 / 180) * cos(ph * 3.1415927 / 180);
        Ey = +2 * dim * sin(ph * 3.1415927 / 180);
        Ez = +2 * dim * cos(th * 3.1415927 / 180) * cos(ph * 3.1415927 / 180);
        gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, cos(ph * 3.1415927 / 180), 0);
    }
    else if (mode == 2) // First Person view
    {
        Ex = 0;
        Ey = 0.5;
        Ez += 0 * 1;
        Cx = Ex + th * 0.05;
        Cy = Ey - cos(ph * 3.1415927 / 180);
        Cz = 0 - Ez;
        gluLookAt(Ex, Ey, Ez, Cx, Cy, Cz, 0, 1, 0);
    }
    //  Draws 2 houses and a plane
    house(1,0,-4,0.6,0.6,0.3);
	house(4,0,-4,0.6,0.6,0.3);
	house(-2,0,-4,0.6,0.6,0.3);
    house(1,-.25,0,0.4,0.4,0.2);
	house(4,-.25,0,0.4,0.4,0.2);
	house(-2,-.25,0,0.4,0.4,0.2);
	car(0.0,-.25,-2.0,.5,.5,.25);
	car(3,-.25,-2.0,.5,.5,.25);
    glFlush();
    glutSwapBuffers();
 }

 /*
  *  GLUT calls this routine when an arrow key is pressed
  */
 void special(int key,int x,int y)
 {
    if (key == GLUT_KEY_RIGHT)
       th -= 5;
    else if (key == GLUT_KEY_LEFT)
       th += 5;
    else if (key == GLUT_KEY_UP)
       ph += 5;
    else if (key == GLUT_KEY_DOWN)
       ph -= 5;
    th %= 360;
    ph %= 360;
    projection();
    glutPostRedisplay();
 }

 /*
  *  GLUT calls this routine when a key is pressed
  */
 void key(unsigned char ch, int x, int y)
{
    if (ch == 27)
        exit(0);
    else if (ch == '0')
        th = ph = 0;
    // Switch display mode
    else if (ch == '1') // First Person view
        mode = 2;
    else if (ch == '2') // Perspective view
        mode = 1;
    else if (ch == '3') // Orthogonal view
        mode = 0;
	else if (ch == '4') 
		dim += .1;
	else if (ch == '5')
		dim -= .1;
    projection();
    glutPostRedisplay();
}

 /*
  *  GLUT calls this routine when the window is resized
  */
 void reshape(int width,int height)
 {
    asp = (height>0) ? (double)width/height : 1;
    glViewport(0,0, width,height);
    projection();
 }


 /*
  *  Start up GLUT and tell it what to do
  */
 int main(int argc,char* argv[])
 {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("Scene in 3D");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutMainLoop();
    return 0;
 }