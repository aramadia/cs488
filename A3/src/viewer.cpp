#include "viewer.hpp"
#include "algebra.hpp"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

Viewer::Viewer()
{
  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Ask for an OpenGL Setup with
  //  - red, green and blue component colour
  //  - a depth buffer to avoid things overlapping wrongly
  //  - double-buffered rendering to avoid tearing/flickering
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                     Gdk::GL::MODE_DEPTH |
                                     Gdk::GL::MODE_DOUBLE);
  if (glconfig == 0) {
    // If we can't get this configuration, die
    std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
    abort();
  }

  // Accept the configuration
  set_gl_capability(glconfig);

  // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      | 
             Gdk::BUTTON_RELEASE_MASK    |
             Gdk::VISIBILITY_NOTIFY_MASK);

  reset_all();
}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::reset_all() {
  DEBUG_MSG("reset all");
  m_trackballTranslation = Matrix4x4();//translation(Vector3D(0.0, 0.0, -15.0));
  m_trackballRotation = Matrix4x4();

  m_lastMouse = Point2D();

  invalidate();

}

void Viewer::invalidate()
{
  // Force a rerender
  Gtk::Allocation allocation = get_allocation();

  if (get_window()) {
    get_window()->invalidate_rect( allocation, false);
  }
}

void Viewer::on_realize()
{
  // Do some OpenGL setup.
  // First, let the base class do whatever it needs to
  Gtk::GL::DrawingArea::on_realize();
  
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
  
  if (!gldrawable)
    return;

  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  glShadeModel(GL_SMOOTH);
  glClearColor( 0.4, 0.4, 0.4, 0.0 );
  glEnable(GL_DEPTH_TEST);


  gldrawable->gl_end();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Set up for perspective drawing 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, get_width(), get_height());
  gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Clear framebuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  GLfloat lightpos[] = {0.5f, 1.f, 10.f, 0.f};
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  // Draw stuff
  GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
  glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  GLfloat shininess[] = {25};
  glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

  glPushMatrix();

    glLoadMatrixd(m_trackballTranslation.begin());
    glPushMatrix();

      glTranslated(0.0, 0.0, -10.0);

      //glTranslated(0.0, 0.0, -20.0);
      GLUquadricObj * quadric = gluNewQuadric();
      gluSphere(quadric, 2.f, 30, 30);

    glPopMatrix();

  glPopMatrix();

  draw_trackball_circle();

  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.
  gldrawable->swap_buffers();

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;
  
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Set up perspective projection, using current size and aspect
  // ratio of display

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, event->width, event->height);
  gluPerspective(40.0, (GLfloat)event->width/(GLfloat)event->height, 0.1, 1000.0);

  // Reset to modelview matrix mode
  
  glMatrixMode(GL_MODELVIEW);

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  DEBUG_MSG( "Stub: Button " << event->button << " pressed" );

  // togle x,y,z based stuff
  //m_mouseLastPos = event->x;

  m_lastMouse = Point2D(event->x, event->y);

  invalidate();
  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  DEBUG_MSG(" Stub: Button " << event->button << " released");

  invalidate();

  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  DEBUG_MSG("Stub: Motion at " << event->x << ", " << event->y );

  Point2D dMouse(event->x - m_lastMouse[0], event->y - m_lastMouse[1]);

  bool b1,b2,b3;
  b1 = event->state & GDK_BUTTON1_MASK;
  b2 = event->state & GDK_BUTTON2_MASK;
  b3 = event->state & GDK_BUTTON3_MASK;

  // do x,y translation
  if (b1) {


    Vector3D translate(dMouse[0] / 30.0, dMouse[1] / 23.0 , 0);

    Matrix4x4 t = translation(translate);
    m_trackballTranslation = m_trackballTranslation * t;

  }

  if (b2) {

    Vector3D translate(0.0, 0.0, dMouse[1]/ 35.0);

    Matrix4x4 t = translation(translate);
    m_trackballTranslation = m_trackballTranslation * t;
  }


  DEBUG_MSG("trackballMatrix: " << m_trackballTranslation);
  m_lastMouse = Point2D(event->x, event->y);

  invalidate();
  return true;
}

void Viewer::draw_trackball_circle()
{
  int current_width = get_width();
  int current_height = get_height();
  
  // Set up for orthogonal drawing to draw a circle on screen.
  // You'll want to make the rest of the function conditional on
  // whether or not we want to draw the circle this time around.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, current_width, current_height);
  glOrtho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);

  // Reset modelview matrix
  glLoadIdentity();

  // draw a circle for use with the trackball 
  glDisable(GL_LIGHTING);
  glEnable(GL_LINE_SMOOTH);
  glColor3f(1.0, 1.0, 1.0);
  double radius = current_width < current_height ? 
    (float)current_width * 0.25 : (float)current_height * 0.25;
  glTranslated((float)current_width * 0.5, (float)current_height * 0.5, 0);
  glBegin(GL_LINE_LOOP);
  for(size_t i=0; i<40; ++i) {
    double cosine = radius * cos(i*2*M_PI/40);
    double sine = radius * sin(i*2*M_PI/40);
    glVertex2f(cosine, sine);
  }
  glEnd();
  glColor3f(0.0, 0.0, 0.0);
  glDisable(GL_LINE_SMOOTH);
}
