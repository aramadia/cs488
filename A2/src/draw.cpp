/****************************************************************************
 *
 * CS488 -- Introduction to Computer Graphics
 *
 * draw.hpp/draw.cpp
 *
 * In Assignment 2, this code is responsible for drawing lines
 * in an OpenGL context.  We put it in a separate file to emphasize
 * that you shouldn't be using any OpenGL directly -- your drawing
 * function should only call functions declared here.
 *
 * University of Waterloo Computer Graphics Lab / 2003-2005
 *
 ****************************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "draw.hpp"

void draw_line(const Point2D& p, const Point2D& q)
{
  glVertex2d(p[0], p[1]);
  glVertex2d(q[0], q[1]);
}

void set_colour(const Colour& col)
{
  glColor3f((float)col.R(), (float)col.G(), (float)col.B());
}

void draw_init(int width, int height)
{
  glClearColor(0.7, 0.7, 0.7, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double)width, 0.0, (double)height);
  glViewport(0, 0, width, height);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(0.0, (double)height, 0.0);
  glScaled(1.0, -1.0, 1.0); 

  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(1.0);

  glBegin(GL_LINES);
}

void draw_complete()
{
  glEnd();
}
