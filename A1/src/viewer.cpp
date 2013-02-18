// Daniel Lam
// dalam
// 20297494

#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

#include "debug.hpp"

Viewer::Viewer():
  m_game(10, 20),
  m_mode(WIRE),
  m_double_buffer_enabled(false),
  m_rotX(0.0),
  m_rotY(0.0),
  m_rotZ(0.0),
  m_scale(1.0),
  m_rotVX(0.0),
  m_rotVY(0.0),
  m_rotVZ(0.0),
  m_resetTimer(0),
  m_last_angle(0.0)
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

  m_game.reset();
  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &Viewer::tick), 500);


  // trigger a render at 60 FPS 
  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &Viewer::refresh), 15);

  // Accept the configuration
  set_gl_capability(glconfig);

  // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      | 
             Gdk::BUTTON_RELEASE_MASK    |
             Gdk::VISIBILITY_NOTIFY_MASK);
}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::setTimer(int ms) {
  m_resetTimer = ms;
}

bool Viewer::tick() {

  if (m_resetTimer) {

    Glib::signal_timeout().connect(
      sigc::mem_fun(*this, &Viewer::tick), m_resetTimer);
    m_resetTimer = 0;
    return false;
  }

  invalidate();
  int status = m_game.tick();

  if (status < 0) {
    // game over
    DEBUG_MSG("Game Over");
  }
  return true;  // keeps the timer running
}


void Viewer::reset() {
  m_rotX = m_rotY = m_rotZ = 0.0;
  m_rotVX = m_rotVY = m_rotVZ = 0.0;  
  m_scale = 1.0;
  invalidate();
}

void Viewer::new_game() {
  m_game.reset();
  invalidate();
}

void Viewer::invalidate()
{
  //Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
  
}

bool Viewer::refresh() {
  invalidate();
  return true;
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

  // Just enable depth testing and set the background colour.
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.7, 0.7, 1.0, 0.0);

  gldrawable->gl_end();
}

void Viewer::drawCube(double x, double y, double z) {

  glPushMatrix();

  glTranslated(x, y, z);
  glScaled(0.5, 0.5, 0.5);

  glBegin(GL_TRIANGLES);
  // cube 6 faces, 8 points
  // 12 triangles
  // 36 verticies

  // top 
  // (-1, 1, -1) TL
  // (1, 1, -1) TR
  // (1, 1, 1) BR
  // (-1, 1, 1) BL

  // bottom (-1, -1, -1), (1, 1, -1), (1, -1, 1), (-1, 1, 1)

  // top face ()
  if (m_mode == MULTI) {
    glColor3d(1.0,0.0,0.0);
  }
  glVertex3d(-1, 1, -1);  // UTL  
  glVertex3d(1, 1, 1);    // UBR
  glVertex3d(1, 1, -1);   // UTR

  glVertex3d(-1, 1, 1);  // UBL 
  glVertex3d(1, 1, 1);    // UBR
  glVertex3d(-1, 1, -1);  // UTL

  // bottom face y = -1  
  // Quad (1, -1, 1), (1,-1,-1), (-1, -1, -1), (-1, -1, 1)
  if (m_mode == MULTI) {
    glColor3d(0.0,1.0,0.0);
  }


  glVertex3d(1, -1, 1);
  glVertex3d(1, -1, -1);
  glVertex3d(-1, -1, 1);

  glVertex3d(1, -1, -1);
  glVertex3d(-1, -1, -1);
  glVertex3d(-1, -1, 1);

  // front face  z = 1 (-1, 1), (-1, -1), (1, -1), (1, 1)

  if (m_mode == MULTI) {
    glColor3d(0.0,0.0,1.0);
  }

  glVertex3d(-1, 1, 1);
  glVertex3d(-1, -1, 1);
  glVertex3d(1, -1, 1);
  glVertex3d(1, -1, 1);
  glVertex3d(1, 1, 1);
  glVertex3d(-1, 1, 1);

  // back face z = -1    

  if (m_mode == MULTI) {
    glColor3d(1.0,1.0,0.0);
  }
  glVertex3d(1, -1, -1);
  glVertex3d(-1, -1, -1);
  glVertex3d(-1, 1, -1);

  glVertex3d(-1, 1, -1);
  glVertex3d(1, 1, -1);
  glVertex3d(1, -1, -1);

  // right face  x = 1 ()

  if (m_mode == MULTI) {
    glColor3d(1.0,0.0,1.0);
  }

  glVertex3d(1, -1, 1);
  glVertex3d(1, -1, -1);
  glVertex3d(1, 1, -1);
  glVertex3d(1, 1, -1);
  glVertex3d(1, 1, 1);
  glVertex3d(1, -1, 1);

  // left face x = -1
  // QUAD (-1, 1 - 1), ()

  if (m_mode == MULTI) {
    glColor3d(0.0,1.0,1.0);
  }

  glVertex3d(-1, 1, -1);
  glVertex3d(-1, -1, -1);
  glVertex3d(-1, -1, 1);

  glVertex3d(-1, -1, 1);
  glVertex3d(-1, 1, 1);
  glVertex3d(-1, 1, -1);



  glEnd();

  glPopMatrix();
}

void Viewer::set_draw_mode(DrawMode mode) {
  DEBUG_MSG("changed draw mode to " << mode);
  m_mode = mode;
  invalidate();

}

void Viewer::toggle_double_buffer() {
  m_double_buffer_enabled = !m_double_buffer_enabled;
  DEBUG_MSG("changed double buffering to " << m_double_buffer_enabled);


}


bool Viewer::on_expose_event(GdkEventExpose* event)
{

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;
  
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;
  


  if (!m_double_buffer_enabled) {
    DEBUG_MSG("Draw to front only");
    glDrawBuffer(GL_FRONT);
  }
  else {
    DEBUG_MSG("Draw to back only");
    glDrawBuffer(GL_BACK);
  }

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_mode == WIRE) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  }
  else {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }

  // Modify the current projection matrix so that we move the 
  // camera away from the origin.  We'll draw the game at the
  // origin, and we need to back up to see it.

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glTranslated(0.0, 0.0, -40.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Not implemented: set up lighting (if necessary)

  // scale and rotate the scene
  glRotated(m_rotX, 1.0, 0.0, 0.0);
  glRotated(m_rotY, 0.0, 1.0, 0.0);
  glRotated(m_rotZ, 0.0, 0.0, 1.0);
  glScaled(m_scale, m_scale, m_scale);

  // add persitence
  m_rotX += m_rotVX;
  m_rotY += m_rotVY;
  m_rotZ += m_rotVZ;

  // You'll be drawing unit cubes, so the game will have width
  // 10 and height 24 (game = 20, stripe = 4).  Let's translate
  // the game so that we can draw it starting at (0,0) but have
  // it appear centered in the window.
  glTranslated(-5.5, -12.0, 0.0);

  // draw A U Well (holds 20 vertical and 10 horizontal)

  glColor3d(0.2, 0.2, 0.2);
  for (int y = 1; y <= 20; y++) {
    drawCube(0, y, 0);
    drawCube(11, y, 0);
  }

  for (int x = 0; x <= 11; x++) {
    drawCube(x, 0, 0);
  }

  // draw the game board
  for (int y = 0; y < m_game.getHeight() + 4; y++) {
    for (int x =0; x < m_game.getWidth(); x++) {
      int content = m_game.get(y, x);
      if (content >= 0) {
        if (m_mode == FACE) {
          switch(content) {
          case 0:
            glColor3d(1.0,0.0,0.0);
            break;
          case 1:
            glColor3d(0.0,1.0,0.0);
            break;
          case 2:
            glColor3d(0.0,0.0,1.0);
            break;
          case 3:
            glColor3d(1.0,1.0,0.0);
            break;
          case 4:
            glColor3d(1.0,0.0,1.0);
            break;
          case 5:
            glColor3d(0.0,1.0,1.0);
            break;
          case 6:
            glColor3d(1.0,1.0,1.0);
            break;
          default:
            glColor3d(0.0,0.0,0.0);
          }
        }
        drawCube(x + 1, y + 1, 0);
      }
    }
  }


  // We pushed a matrix onto the PROJECTION stack earlier, we 
  // need to pop it.

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.

  if (m_double_buffer_enabled) {
    DEBUG_MSG("swap buffer");
    gldrawable->swap_buffers();   

  }
  else {
    glFlush();
  }

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
  DEBUG_MSG("Stub: Button " << event->button << " pressed");
  m_mouseLastPos = event->x;


  m_rotVX = m_rotVY = m_rotVZ = 0.0;
  m_last_angle = 0.0;

  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  DEBUG_MSG("Stub: Button " << event->button << " released at " << event->x << ", " << event->y);

  double dAngle = m_last_angle;
  DEBUG_MSG("setting rot v at " << dAngle);
  if (event->state & GDK_BUTTON1_MASK) {
    m_rotVX = dAngle;
  }
  if (event->state & GDK_BUTTON2_MASK) {
    m_rotVY = dAngle;
  }
  if (event->state & GDK_BUTTON3_MASK) {
    m_rotVZ = dAngle;
  }

  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  DEBUG_MSG( "Stub: Motion at " << event->x << ", " << event->y);

  int dx = event->x - m_mouseLastPos;
  m_mouseLastPos = event->x;


  double dAngle = dx;

  if (event->state &GDK_SHIFT_MASK) {
    DEBUG_MSG("scale " << m_scale);
    m_scale += dAngle * 0.01;

    // bound the min and max scale factor
    if (m_scale < 0.1) {
      m_scale = 0.1;
    } 
    else if (m_scale > 5.0) {
      m_scale = 5.0;
    }
  }
  else {
    if (event->state & GDK_BUTTON1_MASK) {
      m_rotX += dAngle;
    }
    if (event->state & GDK_BUTTON2_MASK) {
      m_rotY += dAngle;
    }
    if (event->state & GDK_BUTTON3_MASK) {
      m_rotZ += dAngle;
    }
    m_last_angle = dAngle;
  }

  invalidate();

  return true;
}

