/* ***************************************************************
 *
 * File : SDL_OGL.cpp
 *
 * Author : Tiberiu Popa
 * 	    J. Alexander Clarke
 * Date   : June 18th, 2002
 *
 * Modified:
 *
 * Purpose: Implementation file for SDl with OpenGL framework
 *
 * Notes: Based on an SDL tutorial from SDL's web site
 *
 * ****************************************************************/
#include "SDL_OGL.h"
#include <stdio.h>



void ReSizeGLScene(GLsizei width, GLsizei height){
    
    // Reset The Current Viewport
    glViewport(0,0,width,height);	

    glMatrixMode(GL_PROJECTION);					
    glLoadIdentity();									

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(40.0, (GLfloat)width/(GLfloat)height, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);							
    glLoadIdentity();								

}


int InitGLLights(){
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

// Initialization
int InitGL(GLvoid){
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	// Black Background
	glClearColor(0.00f, 0.80f, 0.80f, 0.0f);			
	
	glEnable(GL_DEPTH_TEST);					
	
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	// initialize the lighs
	InitGLLights();

    return 1;
}


GLvoid KillGLWindow(GLvoid){
	SDL_Quit();
}


int CreateGLWindow(char* title, int width, int height, int bits, int fullscreenflag)
{
	Uint32 flags;
	int size;

	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
		fprintf(stderr, "Couldn't init SDL: %s\n", SDL_GetError());
		return 0;
	}

	flags = SDL_OPENGL;
	if ( fullscreenflag ) {
		flags |= SDL_FULLSCREEN;
	}

	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 1 );
	if ( SDL_SetVideoMode(width, height, 0, flags) == NULL ) {
		return 0;
	}

	SDL_GL_GetAttribute( SDL_GL_STENCIL_SIZE, &size);

	ReSizeGLScene(width, height);						


	if (!InitGL()){
		KillGLWindow();
		return 0;		
	}

	return 1;
}


