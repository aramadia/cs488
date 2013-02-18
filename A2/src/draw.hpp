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

#ifndef CS488_DRAW_HPP
#define CS488_DRAW_HPP

#include "algebra.hpp"

// Draw a line -- call draw_init first!
void draw_line(const Point2D& p, const Point2D& q);

// Set the current colour
void set_colour(const Colour& col);

// Call this before you begin drawing. Width and height are the width
// and height of the GL window.
void draw_init(int width, int height);

// Call this after all lines have been drawn for one frame
void draw_complete();

#endif // CS488_DRAW_HPP
