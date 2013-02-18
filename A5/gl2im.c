/*
 *----------------------------------------------------------------------
 *  File:  gl2im.c
 *      Im output routines for gl.
 *----------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "pic.h"


void WriteIM(char* name)
{
        Pic* pic;
        long x1,y1,xSize,ySize;
        unsigned long* parray;
        Pic_Pixel* pix;
        char c[3];
        int i,j;

        fprintf(stderr,"starting write...");fflush(stderr);
        getorigin(&x1,&y1);
        getsize(&xSize, &ySize);

        parray = (unsigned long*)malloc(xSize*ySize*sizeof(unsigned long));
        pix = (Pic_Pixel*)malloc(xSize*ySize*sizeof(Pic_Pixel));
        swapbuffers();
        lrectread(0,0,0+xSize-1,0+ySize-1,parray);
        swapbuffers();

        pic = PicOpen(name, xSize, ySize);

        for (i=ySize-1; i>=0; i--) {
                for (j=0; j<xSize; j++) {
                        pix[j].b = (*(parray+i*xSize+j)&0xFF0000)>>16;
                        pix[j].g = (*(parray+i*xSize+j)&0xFF00  )>>8;
                        pix[j].r = (*(parray+i*xSize+j)&0xFF    );
                }
                PicWriteLine(pic, pix);
        }
        free(parray);
        free(pix);
        PicClose(pic);
        fprintf(stderr,"write done.\n");
}
