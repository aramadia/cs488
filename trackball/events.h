/*
 * Module : events.h
 *
 * Author : Greg Veres
 *
 * Date   : April 10, 1995
 *
 * Purpose: Header file for events.c.
 *
 */
#ifndef __EVENTS_H
#define __EVENTS_H

#include <GL/gl.h>

/*
 * Constants
 */
#define  DIR_NONE      0x00 
#define  DIR_X         0x01     
#define  DIR_Y         0x02    
#define  DIR_Z         0x04   

#define  OBJ_CUBE      1
#define  OBJ_PLANE     2

#define  SENS_PANX     30.0
#define  SENS_PANY     23.0
#define  SENS_ZOOM     35.0

typedef GLdouble Matrix[4][4];

extern Matrix mIdentity;

/*
 * Function Prototypes
 */
void vMakePlane(void);
void vMakeCube(void);
void vCopyMatrix(Matrix, Matrix);
void vRightMultiply(Matrix, Matrix);
void vTranslate(float, char, Matrix);
void vDrawScene(void);
void vToggleDir(int);
void vPerformTransfo(float, float, float, float);
void vEventLoop(void);

#endif
