/*
 *  Textures and Lighting
 *
 *  Demonstrates using lighting and textures.
 *
 *  Key bindings:
 *  l          Toggle lighting on/off
 *  t          Change textures
 *  m          Toggles texture mode modulate/replace
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int mode=0;       //  Texture mode
int ntex=0;       //  H faces
int axes=0;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=6;   //  Size of world
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[8]; // Texture names
int Xmode = 0; //Changes between perspective and ortho view
int randInt = 0;


void generateRandomInt() {
	
    randInt = rand() % 8;  // Generates a random integer between 0 and 6
	if (randInt == 2) {
		generateRandomInt();
	}
}

// Allow BMP images of 32 bits
// Define BMP structures
#pragma pack(push, 1)
typedef struct {
    unsigned short type;              // Magic identifier: 0x4d42
    unsigned int size;                // File size in bytes
    unsigned short reserved1, reserved2;
    unsigned int offset;              // Offset to image data in bytes from beginning of file (54 bytes)
} BMPHeader;

typedef struct {
    unsigned int size;                // Header size in bytes (40 bytes)
    int width, height;                // Width and height of image
    unsigned short planes;            // Number of color planes
    unsigned short bits;              // Bits per pixel
    unsigned int compression;         // Compression type
    unsigned int imagesize;           // Image size in bytes
    int xresolution, yresolution;     // Pixels per meter
    unsigned int ncolors;             // Number of colors
    unsigned int importantcolors;     // Important colors
} BMPInfoHeader;
#pragma pack(pop)

GLuint LoadTexBMP(const char* file)
{
    BMPHeader header;
    BMPInfoHeader info;
    unsigned char* data;
    unsigned int texture;
    FILE* f = fopen(file, "rb");

    if (!f) {
        fprintf(stderr, "Cannot open file %s\n", file);
        return 0;
    }

    // Read and check the header
    fread(&header, sizeof(BMPHeader), 1, f);
    if (header.type != 0x4D42) {
        fprintf(stderr, "File %s is not a BMP file\n", file);
        fclose(f);
        return 0;
    }

    // Read and check the info header
    fread(&info, sizeof(BMPInfoHeader), 1, f);
    if (info.bits != 24 && info.bits != 32) {
        fprintf(stderr, "File %s is not a 24-bit or 32-bit BMP file: %d bits per pixel\n", file, info.bits);
        fclose(f);
        return 0;
    }

    // Allocate memory for the image data
    data = (unsigned char*)malloc(info.imagesize);
    if (!data) {
        fprintf(stderr, "Cannot allocate memory for image %s\n", file);
        fclose(f);
        return 0;
    }

    // Read the image data
    fseek(f, header.offset, SEEK_SET);
    fread(data, info.imagesize, 1, f);
    fclose(f);

    // Convert BGR to RGB and handle alpha channel if necessary
    for (unsigned int i = 0; i < info.imagesize; i += (info.bits / 8)) {
        unsigned char tmp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = tmp;
    }

    // Generate a texture ID and bind to it
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture
    if (info.bits == 24) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (info.bits == 32) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    // Generate mipmaps for the texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image data
    free(data);

    return texture;
}

/*
 *  Draw a house
 *     dimensions (dx,dy,dz)
 *     at (x,y,z)
 *     rotated th about the y axis
 */
static void house(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   // Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

    // Save transformation
    glPushMatrix();
    // Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(dx,dy,dz);
    glRotated(th,0,1,0);

    // Enable textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    glColor3f(1.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    // Begin drawing house
	glColor3f(1.0, 1.0, 1.0);
    if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    // Front wall
    glColor3f(1.0, 1.0, 1.0);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
    glTexCoord2f(1,0); glVertex3f(1,-1, 1);
    glTexCoord2f(1,1); glVertex3f(1,1, 1);
    glTexCoord2f(0,1); glVertex3f(-1,1, 1);
    glEnd();
    // Back wall
	glColor3f(1.0, 1.0, 1.0);
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0,0); glVertex3f(1,-1,-1);
	glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
	glTexCoord2f(1,1); glVertex3f(-1,1,-1);
	glTexCoord2f(0,1); glVertex3f(1,1,-1);
	glEnd();
    // Right wall
	glColor3f(1.0, 1.0, 1.0);
    if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(1,-1,1);
    glTexCoord2f(1,0); glVertex3f(1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(1,1,-1);
    glTexCoord2f(0,1); glVertex3f(1,1,1);
    glEnd();
    // Left wall
    glColor3f(1.0, 1.0, 1.0);
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(-1,-1,1);
    glTexCoord2f(1,1); glVertex3f(-1,1,1);
    glTexCoord2f(0,1); glVertex3f(-1,1,-1);
    glEnd();
    // Roof Right
    glColor3f(1.0, 1.0, 1.0);
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(0.5,1.5,0);
    glTexCoord2f(0,0); glVertex3f(1.5,0.5,+1);
    glTexCoord2f(1,0); glVertex3f(1.5,0.5,-1);
    glTexCoord2f(1,1); glVertex3f(0,+2,-1);
    glTexCoord2f(0,1); glVertex3f(0,+2,+1);
    glEnd();
    // Roof Left
    glColor3f(1.0, 1.0, 1.0);
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(-0.5,1.5,0);
    glTexCoord2f(0,0); glVertex3f(-1.5,0.5,-1);
    glTexCoord2f(1,0); glVertex3f(-1.5,0.5,+1);
    glTexCoord2f(1,1); glVertex3f(0,+2,+1);
    glTexCoord2f(0,1); glVertex3f(0,+2,-1);
    glEnd();
    // Bottom cover
    glColor3f(1.0, 1.0, 1.0);  // White color
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(1,-1,1);
    glTexCoord2f(0,1); glVertex3f(-1,-1,1);
    glEnd();
    // Chimney Front
	glColor3f(1.0, 1.0, 1.0);  // White color
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(0.82, 1.25, -0.25);
    glTexCoord2f(0,0); glVertex3f(+0.75,+1, -0.25);
    glTexCoord2f(1,0); glVertex3f(+1,+1, -0.25);
    glTexCoord2f(1,1); glVertex3f(+1,+1.5,-0.25);
    glTexCoord2f(0,1); glVertex3f(+0.75,+1.5, -0.25);
	glEnd();
	
    // Chimney Right
	glColor3f(1.0, 1.0, 1.0);  // White color
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(1, 1.25, -0.32);
    glTexCoord2f(0,0); glVertex3f(+1,+1, -0.25);
    glTexCoord2f(1,0); glVertex3f(+1,+1, -0.5);
    glTexCoord2f(1,1); glVertex3f(+1,+1.5,-0.5);
    glTexCoord2f(0,1); glVertex3f(+1,+1.5, -0.25);
	glEnd();
	
    // Chimney Back
	glColor3f(1.0, 1.0, 1.0);  // White color
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(0.82, 1.25, -0.5);
    glTexCoord2f(0,0); glVertex3f(+1,+1, -0.5);
    glTexCoord2f(1,0); glVertex3f(+0.75,+1, -0.5);
    glTexCoord2f(1,1); glVertex3f(+0.75,+1.5, -0.5);
    glTexCoord2f(0,1); glVertex3f(+1,+1.5,-0.5);
	glEnd();
	
    // Chimney Left
	glColor3f(1.0, 1.0, 1.0);  // White color
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(0.75, 1.25, -0.32);
    glTexCoord2f(0,0); glVertex3f(+0.75,+1, -0.5);
    glTexCoord2f(1,0); glVertex3f(+0.75,+1, -0.25);
    glTexCoord2f(1,1); glVertex3f(+0.75,+1.5, -0.25);
    glTexCoord2f(0,1); glVertex3f(+0.75,+1.5, -0.5);
	glEnd();
	
    // Chimney Top
	glColor3f(1.0, 1.0, 1.0);  // White color
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_QUADS);
    glNormal3f(0.82, 1.5, -0.32);
    glTexCoord2f(0,0); glVertex3f(+0.75,+1.5, -0.25);
    glTexCoord2f(1,0); glVertex3f(+1,+1.5,-0.25);
    glTexCoord2f(1,1); glVertex3f(+1,+1.5,-0.5);
    glTexCoord2f(0,1); glVertex3f(+0.75,+1.5, -0.5);
    glEnd();

    
    // Front roof wall
    glColor3f(1.0, 1.0, 1.0);
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
	glBegin(GL_TRIANGLES);
    glNormal3f(0, 0.4472, 0.8944);
    glTexCoord2f(0,0); glVertex3f(1,1, 1);
    glTexCoord2f(1,0); glVertex3f(0,2, 1);
    glTexCoord2f(0.5,1); glVertex3f(-1,1, 1);
    glEnd();
    // Back roof wall
    glColor3f(1.0, 1.0, 1.0);  // White color
	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[randInt]);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0.4472, -0.8944);
    glTexCoord2f(0,0); glVertex3f(-1,1,-1);
    glTexCoord2f(1,0); glVertex3f(0,2,-1);
    glTexCoord2f(0.5,1); glVertex3f(1,1,-1);
    glEnd();

    // Undo transformations and textures
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();
   gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
      //  Draw scene
	  house(1,0,-4,0.6,0.6,0.3, 0);
	  house(4,0,-4,0.6,0.6,0.3, 0);
	  house(-2,0,-4,0.6,0.6,0.3, 0);
      house(1,-.25,0,0.4,0.4,0.2, 0);
	  house(4,-.25,0,0.4,0.4,0.2, 0);
	  house(-2,-.25,0,0.4,0.4,0.2, 0);
   
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0; //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate");
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle texture mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures mode
   else if (ch == 't') {
      ntex = 1-ntex;
	  generateRandomInt();
   }
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;

   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project(45,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Textures and Lighting");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[0] = LoadTexBMP("Caput.bmp");
   texture[1] = LoadTexBMP("Ruin Gazer.bmp");
   texture[2] = LoadTexBMP("Wicked Gourd.bmp");
   texture[3] = LoadTexBMP("Fiend.bmp");
   texture[4] = LoadTexBMP("Basilisk.bmp");
   texture[5] = LoadTexBMP("Vile Eye.bmp");
   texture[6] = LoadTexBMP("Goop.bmp");
   texture[7] = LoadTexBMP("Copse Haunter.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
