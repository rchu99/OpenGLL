/*
 * Simple program to demonstrate generating coordinates
 * using the Lorenz Attractor
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdarg.h>
 #include <stdbool.h>
 //  OpenGL with prototypes for glext
 #define GL_GLEXT_PROTOTYPES
 #ifdef __APPLE__
 #include <GLUT/glut.h>
 #else
 #include <GL/glut.h>
 #endif

// Global Vars
int th=0;
int ph=0;
double dim=100;



/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;


#define LEN 8192  
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

// Function to check prime number
bool checkPrime(int N)
{
    // initially, flag is set to true or 1
    int flag = 1;

    // loop to iterate through 2 to N/2
    for (int i = 2; i <= N / 2; i++) {

        // if N is perfectly divisible by i
        // flag is set to 0 i.e false
        if (N % i == 0) {
            flag = 0;
            break;
        }
    }

    if (flag) {
        return true;
    }

    return false;
}

/*
 *  Display the scene
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotated(ph,1000,0,0);
   glRotated(th,0,1000,0);


   int i;
   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;
   /*  Time step  */
   double dt = 0.001;

   
   glBegin(GL_LINE_STRIP);
   /*
    *  Integrate 50,000 steps (50 time units with dt = 0.001)
    *  Explicit Euler integration
    */
    for (i=0;i<50000;i++)
    {
       double dx = s*(y-x);
       double dy = x*(r-z)-y;
       double dz = x*y - b*z;
       x += dt*dx;
       y += dt*dy;
       z += dt*dz;
       // Changes the color green if even
       if(i % 2 == 0) {
         glColor3f(0,1,0);
       }
	   // Changes the color to blue if i is prime
       else if (checkPrime(i)){
		 glColor3f(1,0,0);
		} 
		//Changes the color to red if composite and not even
		else {
			glColor3f(0,0,1);
		}
		glVertex3d(x,y,z);
	}
   glEnd();
   //  Draw white lines for diffent axes
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(50,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,50,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,50);
   glEnd();
   //  Label the axes
   glRasterPos3d(50,0,0);
   Print("X");
   glRasterPos3d(0,50,0);
   Print("Y");
   glRasterPos3d(0,0,50);
   Print("Z");
   //  Trying to get this to work
   //glWindowPos2i(5,5);
   glRasterPos3d(0,80,0);
   Print("X Angle, Y Angle=%d,%d" ,th,ph);
   
   glFlush();
   glutSwapBuffers();
}


void key(unsigned char key, int x, int y)
{
  // Exit on ESC
  if (key == 27)
    exit(0);
  else if (key == '1') {
	r += .25;
  }
  else if (key == '2') {
	r -= .25;
  }
  else if (key == '3') {
	s += .25;  
  }
  else if (key == '4') {
	s -= .25;  
  }
  else if (key == '5') {
	b += .25;  
  }
  else if (key == '6') {
	b -= .25;  
  }
  else if (key == '0') {
	  th = 0;
	  ph = 0;
  }
   glutPostRedisplay();
}

void special(int key,int x,int y)
{
   if (key == GLUT_KEY_RIGHT) {
      th += 5;
   }
   else if (key == GLUT_KEY_LEFT) {
      th -= 5;
   }
   else if (key == GLUT_KEY_UP) {
      ph += 5;
   }
   else if (key == GLUT_KEY_DOWN) {
      ph -= 5;
   }
   th %= 360;
   ph %= 360;
   glutPostRedisplay();
}

void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double w2h;
	if (height > 0) {
		w2h = (double) width / height;
	} else {
		w2h = 1.0;
	}
   glViewport(0,0, width,height);
   glMatrixMode(GL_PROJECTION); 
   glLoadIdentity();
   //  Orthogonal box depending on aspect ratio
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
  
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char *argv[])
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(500,500);
  glutCreateWindow("Lorenz Attractor");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
 
  glutMainLoop();

  return 0;
}