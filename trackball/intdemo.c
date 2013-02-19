/*
 * Module : intdemo.c
 *
 * Author : Greg Veres
 *
 * Date   : April 10, 1995
 *
 * Purpose: Contains the main routine for the interaction demo.
 *
 */
#include <stdio.h>
#include "OGLwin.h"
#include "events.h"

/*
 * Name      : void main()
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : The mainline of the interaction demo.
 */
int main() 
{
    /*
     * Open the main viewing window.
     */
    vOpenWindow();

    /*
     * Perform main event loop.
     */
    vEventLoop();
    return 1;
}

