/*
 * Module : OGLwin.h
 *
 * Author : Greg Veres
 *
 * Date   : April 10, 1995
 *
 * Purpose: Header file for OGLwin.c
 *
 */
#ifndef __OGLWIN_H
#define __OGLWIN_H

#include <X11/Xlib.h>

/*
 * Function Prototypes
 */
void vOpenWindow(void);
void vGetNextEvent(XEvent *tEvent);
void vSwapBuffers(void);

/*
 * Variables Exported by OGLwin.c
 */
extern int nWinHeight, nWinWidth;

#endif
