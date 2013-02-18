/* ***************************************************************
 *
 * File : SDL_OGL.h
 *
 * Author : Tiberiu Popa
 * Date   : June 18th, 2002
 *
 * Modified:
 *
 * Purpose: Header file for SDl with OpenGL framework
 *
 * Notes: Based on an SDL tutorial from SDL's web site
 *
 * ****************************************************************/
#ifndef _SDL_OGL__H
#define _SDL_OGL__H
#pragma interface

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>


int CreateGLWindow(char* title, int width, int height, int bits, int fullscreenflag);
GLvoid KillGLWindow(GLvoid);
int InitGL(GLvoid);
void ReSizeGLScene(GLsizei width, GLsizei height);  


#endif // _SDL_OGL__H
