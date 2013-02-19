#ifndef TRACKBALL_H
#define TRACKBALL_H

#include "events.h"   /* For the definition of Matrix */

/* Function prototypes */
void vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter,
                 float *fVecX, float *fVecY, float *fVecZ);
void vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, Matrix mNewMat);

#endif
