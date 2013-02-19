/*
 * Module : OGLwin.c
 *
 * Author : Greg Veres
 *
 * Date   : April 10, 1995
 *
 * Purpose: Contains all the code to deal with Xwindows and opening an OpenGL
 *          context inside an Xwindow
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <OGLwin.h>

/*
 * Variables exported to other modules. They are set in vGetNextEvent
 */
int nWinHeight, nWinWidth;

/* 
 * X window variables needed by a couple of routines 
 */
static Display *display; /* the display the window is on           */
static Window win;       /* the window we are using                */
static GLXContext gc;    /* the OpenGL context associated with win */


/* 
 * These static routines are used by vOpenWindow() to open an Xwindow with 
 * an OpenGL context. They were extracted without modification from OGLMain
 * supplied with assignment #1. 
 */

/*
 * Name      : XVisualInfo *findVisual( Display *display, int componentsize )
 *
 * Parameters: display       - the X display to search for a visual on
 *             componentsize - the number of bits for red, green, blue and
 *                             depth that the visual must support
 *
 * Returns   : XVisualInfo * - a pointer to an X visual info structure
 *                             containing the visual found.
 *
 * Purpose   :  Find an OpenGL visual that is at least componentsize bits 
 *              of colour per red, green and blue. The visual will also be 
 *              single buffer instead of double buffer. The visual is 
 *              returned as a pointer. If the pointer is NULL then there was
 *              an error and the correct visual could not be found.
 */
static XVisualInfo *findVisual( Display *display, int componentsize )
{
    int visualAttribList[11];
    XVisualInfo *visual;

    /* Ask GLX for a misual that matches the attributes we want: Single
       buffered and RGB with at least 4 bits for each component */
    visualAttribList[0] = GLX_RGBA;
    visualAttribList[1] = GLX_RED_SIZE;
    visualAttribList[2] = componentsize;
    visualAttribList[3] = GLX_GREEN_SIZE;
    visualAttribList[4] = componentsize;
    visualAttribList[5] = GLX_BLUE_SIZE;
    visualAttribList[6] = componentsize;
    visualAttribList[7] = GLX_DEPTH_SIZE;
    visualAttribList[8] = componentsize;
    visualAttribList[9] = GLX_DOUBLEBUFFER;
    visualAttribList[10] = None;
    
    visual = glXChooseVisual( display, 
                              DefaultScreen( display ), 
                              visualAttribList );
    if( visual == (XVisualInfo *) 0 ) {
        fprintf( stderr, "Main: Could not find a suitable visual\n" );
        /* at this point visual == NULL so we just return visual to
           indicate an error */
    }
    return( visual );
}

/*
 * Name      : int get_GC( Window win, XVisualInfo *visual, GLXContext *gc )
 *
 * Parameters: win    - the X window use to the OpenGL context with
 *             visual - The visual to create the context for
 *             gc     - a pointer to a GLXContext structure. This is how
 *                      the created context will be returned to the caller
 *
 * Returns   : a pointer to a created GLXContext is returned through the
 *             gc argument.
 *             int - an error code: 0 means everything was fine
 *                                 -1 context creation failed
 *                                 -2 context/window association failed
 *
 * Purpose   : create an X window Graphics context and assocaite it with 
 *             the window. It returns 0 if everything was fine, -1 if the 
 *             context could not be created, -2 if the context could not 
 *             be associated with the window 
 */
static int get_GC( Window win, XVisualInfo *visual, GLXContext *gc )
{
    *gc = glXCreateContext( display, visual, None, True );

    /* check if the context could be created */
    if( *gc == NULL ) {
        fprintf( stderr, "get_GC: Could not allocate an OpenGL context\n");
        return( -1 );
    }
    /* associated the context with the X window */
    if( glXMakeCurrent( display, win, *gc ) == False) {
        fprintf( stderr, 
                 "get_GC: Could not attach the OpenGL context to the X window\n");
        glXDestroyContext( display, *gc );
        return( -2 );
    }
    return (0);
}

/*
 * Name      : Colormap allocateColourmap( Display *display, 
 *                                         XVisualInfo *visual )
 *
 * Parameters: display - the X display to create the colormap on
 *             visual  - The visual to create the colormap for
 *
 * Returns   : colormap - an xwindow colormap allocated for use with the
 *                        supplied visual and display
 *
 * Purpose   : create a colour map to use with the X window. Even though 
 *             the visual may be a 24 bit TrueColor visual, you still need 
 *             a colour map if the visual is not the default visual used by 
 *             the window manager. Most OpenGL visuals are not the default 
 *             visual used by the window manager so just to be safe we
 *              always allocate a colour map. 
 */
static Colormap allocateColourmap( Display *display, XVisualInfo *visual )
{
    /* now we have the visual with the best depth so lets make a colour map
       for it.  we use allocnone because this is a true colour visual and
       the colour map is read only anyway. This must be done because we
       cannot call XCreateSimpleWindow. This is really the gross part of
       working with X windows. */
    return( XCreateColormap( display,
                             RootWindow( display, visual->screen ),
                             visual->visual,
                             AllocNone ) );
}

/*
 * Name      : void vOpenWindow()
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : Creates the main viewing window.
 */
void vOpenWindow() 
{

    unsigned int display_width, display_height;
    unsigned int window_width, window_height, border_width = 2;
    unsigned int keys_buttons;
    int screen;
    int window_x = 0, window_y = 0;
    int x0, y0, x1, y1, rx, ry;
    XSizeHints size_hints;
    char *display_name = NULL;
    XSetWindowAttributes attribs;
    int visualAttribList[10];
    XVisualInfo *visual;


    /* make a connection to the Xwindows display server */
    if( (display=XOpenDisplay(display_name))==NULL ) {
        (void)fprintf( stderr, "Main: cannot connect to X server %s\n",
                       XDisplayName(display_name) );
        exit( -1 );
    }

    /* query some values that we will need later */
    screen         = DefaultScreen( display );
    display_width  = DisplayWidth( display, screen );
    window_width   = display_width / 2;
    display_height = DisplayHeight( display, screen );
    window_height  = display_height / 2;

    /* Check to see if the Xserver supports OpenGL */
    if( !glXQueryExtension(display, (int *) 0, (int *) 0) ) {
        fprintf( stderr, "Main: this X server does not support OpenGL\n" );
        exit( -2 );
    }

    /* find an OpenGL visual that is RGB, single buffered and has at least
       4 bits per colour component of r,g and b (ie. at least 12 bit per
       pixel visual) */
    visual = findVisual( display, 4 );

    /* tell x what events I want the window to accept */
    attribs.event_mask = KeyPressMask | ExposureMask | StructureNotifyMask;

    /* I do not know why but you MUST specify a border pixel value for
       XCreateWindow to work. If you do not do this, you WILL get a run
       time error. */
    attribs.border_pixel = BlackPixel(display,screen);

    attribs.colormap = allocateColourmap( display, visual );

    /* this is the more complicated way of opening an X window but we must
       use it because we want to specifiy the visual that the window is to
       use. This is necessary for OpenGL */
    win = XCreateWindow( display, RootWindow( display, screen ), 
                         window_x, window_y,
                         window_width, window_height,
                         border_width, 
                         visual->depth,
                         InputOutput,
                         visual->visual,
                         CWColormap | CWEventMask | CWBorderPixel,
                         &attribs );

    XStoreName( display, win, "Interaction Demo - Press 'Q' To Quit" );
    XClearWindow( display, win );
    XFlush( display );

    /* This is the call you need to make to tell X which events to pass on
       to the program for tis window. The events are specified in by the
       event masks being or'ed together as the last parameter. To get more
       or less events in this window, add or remove some of the masks. */
    XSelectInput(display, 
                 win, 
                 ExposureMask | KeyPressMask | ButtonPressMask | 
                 ButtonReleaseMask | StructureNotifyMask | ButtonMotionMask);

    if (get_GC( win, visual, &gc ) != 0) {
        XDestroyWindow( display, win );
        XCloseDisplay( display );
        exit( -3 );
    }

    /* we are now done with the visual so we should free the storage */
    XFree( visual );


    XMapWindow( display, win );
}

/*
 * Name      : void vInitGL(int iXSize, int iYSize)
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : Initializes the components of GL
 */
void vInitGL(int iXSize, int iYSize) 
{

    /*
     * Set various modes.
     */
    glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );\
    glColor3i( 0, 0, 0 );

    glViewport(0, 0, iXSize, iYSize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, iXSize/iYSize, 0.1, 1000.0);
/*     lookat(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0); */
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Name      : void vGetNextEvent(XEvent *tEvent)
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : Creates the main viewing window.
 */
void vGetNextEvent(XEvent *tEvent)
{
    /* Get the next event, however, event compression is used */
    /* for MotionNotify events.  MotionNotify events are disregarded */
    /* until the queue is empty. */
    do {
        XNextEvent(display, tEvent);
        if (tEvent->type == ConfigureNotify) {
            vInitGL(tEvent->xconfigure.width, tEvent->xconfigure.height);
            nWinHeight = tEvent->xconfigure.height;
            nWinWidth = tEvent->xconfigure.width;
            continue;
        }
        if (QLength(display) == 0) {
            break;
        }
    } while (tEvent->type == MotionNotify);
}

/*
 * Name      : void vSwapBuffers()
 *
 * Parameters: None.
 *
 * Returns   : void
 *
 * Purpose   : Creates the main viewing window.
 */
void vSwapBuffers()
{
    glXSwapBuffers(display, win);
}
