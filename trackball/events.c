/*
 * Module : events.c
 *
 * Author : Greg Veres
 *
 * Date   : April 10, 1995
 *
 * Purpose: Controls event processing and scene drawing.
 *
 */
#include <stdlib.h>
#include "events.h"
#include "OGLwin.h"
#include "trackball.h"
#include <X11/Xutil.h>

/* 
 * Local Global Variables
 */
static Matrix mRotations    = {{1.0, 0.0, 0.0, 0.0},
                               {0.0, 0.91, 0.42, 0.0},
                               {0.0, -0.42, 0.91, 0.0},
                               {0.0, 0.0, 0.0, 1.0}};
static Matrix mTranslations = {{1.0, 0.0, 0.0, 0.0},
                               {0.0, 1.0, 0.0, 0.0},
                               {0.0, 0.0, 1.0, 0.0},
                               {0.0, 0.0, -25.0, 1.0}};
Matrix mIdentity     = {{1.0, 0.0, 0.0, 0.0},
                        {0.0, 1.0, 0.0, 0.0},
                        {0.0, 0.0, 1.0, 0.0},
                        {0.0, 0.0, 0.0, 1.0}};

static GLfloat fYellowVec[] = {1.0, 1.0, 0.0};
static GLfloat fWhiteVec[]  = {1.0, 1.0, 1.0};
static GLfloat fBlackVec[]  = {0.0, 0.0, 0.0};
static GLfloat fGreenVec[]  = {0.0, 1.0, 0.0};
static GLfloat fBlueVec[]   = {0.0, 0.0, 1.0};
static GLfloat fGrayVec[]   = {0.5, 0.6, 0.5};
static GLfloat fRedVec[]    = {1.0, 0.0, 0.0};

short sXReference, sYReference;

int nCurrentDir = DIR_NONE;

/*
 * Name      : void MakePlane()
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : Creates the plane.
 */
void vMakePlane() 
{
    int nIndex;

    glNewList(OBJ_PLANE, GL_COMPILE);
    {

        /*
         * Draw the grid.
         */
        glBegin(GL_LINES);
        {
            glColor3fv(fGrayVec);
            for (nIndex = 0; nIndex < 41; nIndex++) {
                glVertex3f(-10.0 + 0.5 * nIndex, 0.0, -10.0);
                glVertex3f(-10.0 + 0.5 * nIndex, 0.0, 10);
                glVertex3f(-10.0,                0.0, -10 + 0.5 * nIndex);
                glVertex3f( 10.0,                0.0, -10 + 0.5 * nIndex);
            }

            /*
             * Draw the axes.
             */
            glLineWidth(5.0);

            glColor3fv(fGreenVec);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(5, 0.0, 0.0);

            glColor3fv(fBlueVec);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 5, 0.0);

            glColor3fv(fRedVec);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, 5);

            /*
             * Draw the Y.
             */
            glColor3fv(fBlackVec);
            glVertex3f(0.0, 5.0, 0.0);
            glVertex3f(0.0, 5.5, 0.0);
            glVertex3f(0.0, 5.5, 0.0);
            glVertex3f(-0.5, 6.0, 0.0);
            glVertex3f(0.0, 5.5, 0.0);
            glVertex3f(0.5, 6.0, 0.0);

            /*
             * Draw the Z.
             */
            glVertex3f(-0.5, 0.0, 5.0);
            glVertex3f(0.5, 0.0, 5.0);
            glVertex3f(0.5, 0.0, 5.0);
            glVertex3f(-0.5, 0.0, 6.0);
            glVertex3f(-0.5, 0.0, 6.0);
            glVertex3f(0.5, 0.0, 6.0);

            /*
             * Draw the X.
             */
            glVertex3f(5.0, 0.0, 0.5);
            glVertex3f(6.0, 0.0, -0.5);
            glVertex3f(5.0, 0.0, -0.5);
            glVertex3f(6, 0.0, 0.5);

        }
        glEnd();
        glLineWidth(1.0);
    }
    glEndList();
}

/*
 * Name      : void vMakeCube()
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : Creates a cube object.
 */
void vMakeCube() 
{
    static float v0[3] = {-1.0, -1.0, -1.0};
    static float v1[3] = {-1.0, -1.0,  1.0};
    static float v2[3] = {-1.0,  1.0,  1.0};
    static float v3[3] = {-1.0,  1.0, -1.0};
    static float v4[3] = { 1.0, -1.0, -1.0};
    static float v5[3] = { 1.0, -1.0,  1.0};
    static float v6[3] = { 1.0,  1.0,  1.0};
    static float v7[3] = { 1.0,  1.0, -1.0};

    glNewList(OBJ_CUBE, GL_COMPILE);
    {
        glColor3fv(fBlueVec);
        glBegin(GL_POLYGON);
        {
            glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3);
        }
        glEnd();
        glColor3fv(fBlackVec);
        glBegin(GL_POLYGON);
        {
            glVertex3fv(v0); glVertex3fv(v4); glVertex3fv(v5); glVertex3fv(v1);
        }
        glEnd();
        glColor3fv(fYellowVec);
        glBegin(GL_POLYGON);
        {
            glVertex3fv(v4); glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v5);
        }
        glEnd();
        glColor3fv(fRedVec);
        glBegin(GL_POLYGON);
        {
            glVertex3fv(v3); glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v2);
        }
        glEnd();
        glColor3fv(fGreenVec);
        glBegin(GL_POLYGON);
        {
            glVertex3fv(v5); glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v6);
        }
        glEnd();
        glColor3fv(fGrayVec);
        glBegin(GL_POLYGON);
        {
            glVertex3fv(v0); glVertex3fv(v4); glVertex3fv(v7); glVertex3fv(v3);
        }
        glEnd();
    }
    glEndList();
}


void vTransposeMatrix(Matrix mSrcDst) {
    GLdouble temp;
    int i,j;

    // Transpose matrix
    for ( i=0; i<4; ++i ) {
        for ( j=i+1; j<4; ++j ) {
            temp = mSrcDst[i][j];
            mSrcDst[i][j] = mSrcDst[j][i];
            mSrcDst[j][i] = temp;
        }
    }
}

/*
 * Name      : void vCopyMatrix(Matrix mSource, Matrix mDestination)
 *
 * Parameters: Matrix mSource      - The source matrix.
 *             Matrix mDestination - The destination matrix.
 *
 * Returns   : void
 *
 * Purpose   : Copies matrix mSource to matrix mDestination.
 *             the result in mDestination.
 */
void vCopyMatrix(Matrix mSource, Matrix mDestination) 
{
    int i, j;

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            mDestination[i][j] = mSource[i][j];
        }
    }
}

/*
 * Name      : void vRightMultiply(Matrix mMat1, Matrix mMat2)
 *
 * Parameters: Matrix mMat1 - The first and destination matrix.
 *             Matrix mMat2 - The second matrix.
 *
 * Returns   : void
 *
 * Purpose   : Right multiplies matrix mMat1 by matrix mMat2 and stores
 *             the result in mMat1.
 */
void vRightMultiply(Matrix mMat1, Matrix mMat2) 
{
    int    i, j;
    Matrix mMat3;

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            mMat3[i][j] = mMat1[i][0]*mMat2[0][j] + mMat1[i][1]*mMat2[1][j] +
                mMat1[i][2]*mMat2[2][j] + mMat1[i][3]*mMat2[3][j];
        }
    }
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            mMat1[i][j] = mMat3[i][j];
        }
    }
}

/*
 * Name      : void vTranslate(float fTrans, char cAxis, Matrix mMat)
 *
 * Parameters: float  fAngle - The distance of translation.
 *             char   cAxis  - The axis of rotation.
 *             Matrix mMat   - The matrix to store the result in.
 *
 * Returns   : void
 *
 * Purpose   : Computes the translation along the given axis and stores
 *             the result in mMat.
 */
void vTranslate(float fTrans, char cAxis, Matrix mMat)
{
    vCopyMatrix(mIdentity, mMat);
    switch(cAxis) {
    case 'x':
        mMat[3][0] = fTrans;
        break;

    case 'y':
        mMat[3][1] = fTrans;
        break;

    case 'z':
        mMat[3][2] = fTrans;
        break;
    }
}

/*
 * Name      : void vDrawScene()
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : Draws the entire scene.
 */
void vDrawScene() 
{
    /*
     * Clear the scene.
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    /*
     * Draw the polygons in the scene.
     */
    glPushMatrix();
    glLoadMatrixd((GLdouble *)mTranslations);
    glMultMatrixd((GLdouble *)mRotations);
    glCallList(OBJ_PLANE);
    glCallList(OBJ_CUBE);
    glPopMatrix();

    vSwapBuffers();
}
  
/*
 * Name      : void vToggleDir(int nDir)
 *
 * Parameters: int nDir - The direction.
 *
 * Returns   : void
 *
 * Purpose   : Toggles the direction in the direction vector on 
 *             and off.
 */
void vToggleDir(int nDir) 
{
    nCurrentDir ^= nDir;
}

/*
 * Name      : void vPerformTransfo(float fOldX, float fNewX,
 *                                  float fOldY, float fNewY)
 *
 * Parameters: float fOldX - The previous X value.
 *             float fNewX - The current X value.
 *             float fOldY - The previous Y value.
 *             float fNewY - The current Y value.
 *
 * Returns   : void
 *
 * Purpose   : Updates the various transformation matrices.
 */
void vPerformTransfo(float fOldX, float fNewX, float fOldY, float fNewY) 
{
    float  fRotVecX, fRotVecY, fRotVecZ;
    Matrix mNewMat;
  
    /*
     * Track ball rotations are being used.
     */
    if (nCurrentDir & DIR_Z) {
        float fDiameter;
        int iCenterX, iCenterY;
        float fNewModX, fNewModY, fOldModX, fOldModY;
	
        /* vCalcRotVec expects new and old positions in relation to the center of the
         * trackball circle which is centered in the middle of the window and
         * half the smaller of nWinWidth or nWinHeight.
         */
        fDiameter = (nWinWidth < nWinHeight) ? nWinWidth * 0.5 : nWinHeight * 0.5;
        iCenterX = nWinWidth / 2;
        iCenterY = nWinHeight / 2;
        fOldModX = fOldX - iCenterX;
        fOldModY = fOldY - iCenterY;
        fNewModX = fNewX - iCenterX;
        fNewModY = fNewY - iCenterY;

        vCalcRotVec(fNewModX, fNewModY,
                        fOldModX, fOldModY,
                        fDiameter,
                        &fRotVecX, &fRotVecY, &fRotVecZ);
        /* Negate Y component since Y axis increases downwards
         * in screen space and upwards in OpenGL.
         */
        vAxisRotMatrix(fRotVecX, -fRotVecY, fRotVecZ, mNewMat);

        // Since all these matrices are meant to be loaded
        // into the OpenGL matrix stack, we need to transpose the
        // rotation matrix (since OpenGL wants rows stored
        // in columns)
        vTransposeMatrix(mNewMat);
        vRightMultiply(mRotations, mNewMat);
    }

    /*
     * Pan translations are being used.
     */
    if (nCurrentDir & DIR_X) {
        vTranslate((fNewX - fOldX) / ((float)SENS_PANX), 'x', mNewMat);
        vRightMultiply(mTranslations, mNewMat);
        vTranslate(-(fNewY - fOldY) / ((float)SENS_PANY), 'y', mNewMat);
        vRightMultiply(mTranslations, mNewMat);
    }
   
    /*
     * Zoom translations are being used.
     */
    if (nCurrentDir & DIR_Y) {
        vTranslate((fNewY - fOldY) / ((float)SENS_ZOOM), 'z', mNewMat);
        vRightMultiply(mTranslations, mNewMat);
    }
}

/*
 * Name      : void vEventLoop()
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : The main event loop.
 */
void vEventLoop() 
{

    short     sXCurrent, sYCurrent;
    float     fDummyX, fDummyY, fDummyZ;
    XEvent    tEvent;
    KeySym    tKey;
    int       i;
    char      caText[10];

    /*
     * Create the objects to be drawn in the scene.
     */
    nCurrentDir = DIR_NONE;
    vMakePlane();
    vMakeCube();
    vDrawScene();

    /*
     * Process events forever.   
     */
    while (1) {

        /* 
         * Process the incoming event. 
         */
        vGetNextEvent(&tEvent);
        /*
         * Determine the event that has occurred.
         */
        switch (tEvent.type) {

            /*
             * Redraw the window.
             */
        case Expose:
            if (tEvent.xexpose.count == 0) {
                vDrawScene();
            }
            break;

            /*
             * A mouse button was pressed.
             */
        case ButtonPress:
        case ButtonRelease:
            if (tEvent.xbutton.button == Button1) {
                vToggleDir(DIR_X);
            }
            if (tEvent.xbutton.button == Button2) {
                vToggleDir(DIR_Y);
            }
            if (tEvent.xbutton.button == Button3) {
                vToggleDir(DIR_Z);
            }

            /*
             * Get the reference point and if the button is being
             * pressed, initialize the rotation vector.
             */
            sXReference = tEvent.xbutton.x;
            sYReference = tEvent.xbutton.y;

            /*if (tEvent.type == ButtonPress) {
                vGenerateRotVec((float)sXReference, (float)sYReference,
                                &fDummyX, &fDummyY, &fDummyZ);
            } else {*/
            /*}*/

                vDrawScene();
            break;

            /*
             * The Q key was pressed so exit.
             */
        case KeyPress:
            /* exit by hitting the "q" key */
            i = XLookupString( (XKeyEvent*)&tEvent, caText, 10, &tKey, 0 );
            if ( i == 1 && caText[0] == 'q') {
                exit(0);
            }
            break;

        case MotionNotify:
            /*
             * Perform transformation and redraw.
             */
            if (nCurrentDir) {
                sXCurrent     = tEvent.xmotion.x;
                sYCurrent     = tEvent.xmotion.y;
                vPerformTransfo((float)sXReference, (float)sXCurrent,
                                (float)sYReference, (float)sYCurrent);
                sXReference   = sXCurrent;
                sYReference   = sYCurrent;
                vDrawScene();
            }
            /*
             * Default case - ignore. 
             */
        default:
            break;
        }
    }
}
