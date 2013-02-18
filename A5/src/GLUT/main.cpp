/* ***************************************************************
 *
 * File :  main.cpp
 *
 * Author : Tiberiu Popa
 *  	    J. Alexander Clarke
 * Date   : June 18th, 2002
 *
 * Modified:
 *
 * Purpose: Simple OpenGL program to illustrate the use of GLui with OpenGL and SDL for sound only
 *
 * ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>


#include "SoundManager.h"

#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#ifdef WIN32
#include "glui.h"
#else
//#include <GL/glui.h>
#endif /* WIN32 */

//-------------------------------------------------------------------
// defines
//-------------------------------------------------------------------
#define CALLBACK_QUIT 'q'

#define MICKEY 1
//-------------------------------------------------------------------
// GLUT data 
//-------------------------------------------------------------------
int scrWidth, scrHeight;
float cur_x = -1;
float cur_y = 0;
int buttons[5] = {GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP};

int SND_ID_1 , SND_ID_2, SND_ID_3, MUS_ID_4;

float xpos = 0, ypos = 0;

int music_on = 0;
//--------------------------------------------------------------------
//  State variables
//--------------------------------------------------------------------

void makeMickey()
	{
        GLUquadricObj* qsphere = gluNewQuadric();
	glNewList(MICKEY, GL_COMPILE);

    	gluQuadricDrawStyle(qsphere, GLU_FILL);
    	gluQuadricNormals(qsphere, GLU_SMOOTH);
    	gluQuadricOrientation(qsphere, GLU_OUTSIDE);
    	gluQuadricTexture(qsphere, GL_FALSE);

    	glColor3f(1,1,0);
    	gluSphere(qsphere, 13, 20, 20);
    	glTranslatef(14,14,0);
    	gluSphere(qsphere, 10, 20, 20);
    	glTranslatef(-28,0,0);
    	gluSphere(qsphere, 10, 20, 20);
    	glEndList();
        gluDeleteQuadric(qsphere);
	}


//-------------------------------------
// fix lights
//-------------------------------------

void lights(){

    GLfloat light_position1[] = {50, 50, 0, 1};
    GLfloat light1[] = {0.5, 0.5, 0.5, 1};
    GLfloat light2[] = {0.5, 0.5, .5, 1.0};
    GLfloat zero[] = {0, 0, 0 , 0};

    // setup 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 25);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light2);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    

}

//-------------------------------------------------------------------
// Custom render function
//-------------------------------------------------------------------

void render(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity(); // Reset the view

    // move the scene 
    glTranslatef(xpos, ypos, -150);
    // draw s simple sphere
    glCallList(MICKEY);

}


//-------------------------------------------------------------------
// motion 
//-------------------------------------------------------------------
void motion(int x, int y){

    if(cur_x<0){
	cur_x = x;
	cur_y = y;
    }
 
    xpos  += (float)(x-cur_x)/10.0f;
    ypos  += -(float)(y-cur_y)/10.0f;

    cur_x = x;
    cur_y = y;
 

  glutPostRedisplay();
  }

//-------------------------------------------------------------------
// mouse 
//-------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
  {

      if(button>0){

	  if(state==GLUT_DOWN && buttons[button]==GLUT_UP)
	      SM.PlaySound(button);

	  if(state==GLUT_UP && buttons[button]==GLUT_DOWN)
	      SM.StopSound(button);
      } else {
	  if(state==GLUT_DOWN){
	      if(!music_on){
		  music_on = 1;
		  SM.PlayMusic(MUS_ID_4);
	      }	else {
		  music_on = 0;
		  SM.StopMusic(MUS_ID_4); 
	      }
	  } 
      }

      buttons[button] = state;
   
      glutPostRedisplay();

  }
  
//-------------------------------------------------------------------
// display 
//-------------------------------------------------------------------
void display(void)
  {
  
	/* set up for perspective drawing 
     */
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, scrWidth, scrHeight);
	gluPerspective(40.0, (GLfloat)scrWidth/(GLfloat)scrHeight, 0.1, 1000.0);
	
	/* change to model view for drawing
     */
	glMatrixMode(GL_MODELVIEW);

	/* Reset modelview matrix
     */
	glLoadIdentity();

	/* Clear framebuffer (both colour and depth buffers)
     */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Set up light sources
     */

	// set up the track ball
	
	// enable rescalling of the normals
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);


	render();

	// dump the whole buffer onto the screen should fix my bug
	glFinish();
	glutSwapBuffers();

  }

//-------------------------------------------------------------------
// keyboard 
//-------------------------------------------------------------------
void keyboard(unsigned char k, int x, int y)
  {
  switch(k)
    {
    case CALLBACK_QUIT:
      exit(0);
      break;
    }


  glutPostRedisplay();
  }

//-------------------------------------------------------------------
// init 
//-------------------------------------------------------------------
void init(int argc, char** argv)
  {
      glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	// Black Background
	glClearColor(0.00f, 0.80f, 0.80f, 0.0f);			
	
	glEnable(GL_DEPTH_TEST);					

	lights();
	
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	makeMickey();

  }

//-------------------------------------------------------------------
// reshape 
//-------------------------------------------------------------------
void reshape(int w, int h)
  {
  scrWidth = w;
  scrHeight = h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(40.0, (GLfloat)w/(GLfloat)h, 0.1, 1000.0);

  /* Reset to modelview matrix mode to avoid confusion later
   */
  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();

  }

//-------------------------------------------------------------------
// main 
//-------------------------------------------------------------------
int main(int argc, char** argv){
      
    int main_window;
    scrWidth = 700;
    scrHeight = 700;
      

    SND_ID_1 = SM.LoadSound("card.wav");
    SND_ID_2 = SM.LoadSound("OBS.wav");
    SND_ID_3 = SM.LoadSound("ghost.wav");
    MUS_ID_4 = SM.LoadMusic("UNREAL.S3M");

    // intialize glut and main window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(scrWidth, scrHeight);
    main_window = glutCreateWindow("Render Test");
	
    // initialize callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    init(argc, argv);

    reshape(scrWidth, scrHeight);
  

    glutMainLoop();
  
    return 0;
}

