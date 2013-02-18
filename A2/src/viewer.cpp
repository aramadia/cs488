#include "viewer.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <GL/gl.h>
#include <GL/glu.h>
#include <algorithm>
#include "draw.hpp"
#include "a2.hpp"
#include "appwindow.hpp"

using namespace std;

Viewer::Viewer():
  m_window(NULL),
  m_status(NULL)
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

  m_cube[0] = Point3D(-1, 1, -1);
  m_cube[1] = Point3D(1, 1, -1);
  m_cube[2] = Point3D(1, 1, 1);
  m_cube[3] = Point3D(-1, 1, 1);
  m_cube[4] = Point3D(-1, -1, -1);
  m_cube[5] = Point3D(1, -1, -1);
  m_cube[6] = Point3D(1, -1, 1);
  m_cube[7] = Point3D(-1, -1, 1);

  m_gnomon[0] = Point3D(0,0,0);
  m_gnomon[1] = Point3D(0.5,0,0);
  m_gnomon[2] = Point3D(0,0.5,0);
  m_gnomon[3] = Point3D(0,0,0.5);

}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::set_interaction_mode(InteractionMode mode) {
  DEBUG_MSG("changed mode to " << mode);
  m_mode = mode;
  update_status();

}

void Viewer::invalidate()
{
  if (get_window()) {
    // Force a rerender
    Gtk::Allocation allocation = get_allocation();
    get_window()->invalidate_rect( allocation, false);
  }
}

void Viewer::set_perspective(double fov, double aspect,
                             double near, double far)
{
  // convert fov to radian
  fov = fov * (2 * M_PI / 360.0);

  double f = 1.0/tan(fov/ 2);
  // Fill me in!
  Vector4D r1(f/aspect, 0, 0, 0);
  Vector4D r2(0, f, 0, 0);
  Vector4D r3(0, 0, (far + near)/ (far - near), (-2.0 * far * near)/ (far - near));
  Vector4D r4(0, 0, 1.0, 0);
  m_projectionM = Matrix4x4(r1,r2,r3,r4);

}

void Viewer::reset_view()
{
  {
    
    m_modelM = Matrix4x4();
    // offset at beginning
    m_modelM = m_modelM * translation(Vector3D(.3, .3, 0));
  }

  Vector4D r1(1, 0, 0, 0);
  Vector4D r2(0, 1, 0, 0);
  Vector4D r3(0, 0, 1, 10);
  Vector4D r4(0, 0, 0, 1);
  m_viewM = Matrix4x4(r1,r2,r3,r4);

  if (m_window)
    m_window->set_mode(3);
  m_projectionM = Matrix4x4();
  m_scalingM = Matrix4x4();

  m_viewport[0] = Point2D(get_width() * 0.05, get_height() * 0.05);
  m_viewport[1] = Point2D(get_width() * 0.95, get_height() * 0.95);

  m_fov = 30;
  m_near = 2;
  m_far = 20;

  set_perspective(m_fov, 1, m_near, m_far);

  invalidate();
  DEBUG_MSG("View: " << m_viewM);

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

  gldrawable->gl_end();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Here is where your drawing code should go.
  
  draw_init(get_width(), get_height());
  
  /* A few of lines are drawn below to show how it's done. */

  set_colour(Colour(0.1, 0.1, 0.1));
  
  set_perspective(m_fov, 1, m_near, m_far);

  // draw the viewport
  Point2D viewA(m_viewport[1][0], m_viewport[0][1]);
  Point2D viewB(m_viewport[0][0], m_viewport[1][1]);
  DEBUG_MSG("viewport: " << viewA << " " << m_viewport[0]);
  draw_line(m_viewport[0], viewA);
  draw_line(viewA, m_viewport[1]);
  draw_line(m_viewport[1], viewB);
  draw_line(viewB, m_viewport[0]);

  // 
  // scale the cube
  Point3D temp[8];
  for (int i = 0; i < 8; i++) {
    temp[i] = m_viewM * m_modelM *m_scalingM* m_cube[i];

    DEBUG_MSG("temp " << i << ": " << temp[i]);
  }
  drawCube(temp);

  //draw model gnomon
  set_colour(Colour(1.0, 0, 0));
  Point3D gnomon[4];
  for (int i = 0; i < 4; i++) {
    gnomon[i] = m_viewM * m_modelM * m_gnomon[i];
  }
  drawGnomon(gnomon);

  // draw view gnomon
  set_colour(Colour(0.0, 0.0, 1.0));
  for (int i = 0; i < 4; i++) {
    gnomon[i] = m_viewM * m_gnomon[i];
  }
  drawGnomon(gnomon);

  draw_complete();
            
  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.
  gldrawable->swap_buffers();

  gldrawable->gl_end();

  update_status();

  return true;
}

void Viewer::drawCube(Point3D cube[8]) {

  set_colour(Colour(0.0, 1.0, 1.0));
  drawLine(cube[0], cube[1]);
  drawLine(cube[1], cube[2]);
  drawLine(cube[2], cube[3]);
  drawLine(cube[3], cube[0]);


  set_colour(Colour(0.0, 0.0, 0.0));
  drawLine(cube[4], cube[5]);
  drawLine(cube[5], cube[6]);
  drawLine(cube[6], cube[7]);
  drawLine(cube[7], cube[4]);


  drawLine(cube[0], cube[4]);
  drawLine(cube[1], cube[5]);
  drawLine(cube[2], cube[6]);
  drawLine(cube[3], cube[7]);

}

void Viewer::drawGnomon(Point3D gnomon[4]) {

  drawLine(gnomon[0], gnomon[1]);
  drawLine(gnomon[0], gnomon[2]);
  drawLine(gnomon[0], gnomon[3]);

}

Point3D Viewer::toViewport(Point3D p ) {
  // equivalent to glViewport
  return Point3D(
    (p[0] + 1) *
            (m_viewport[1][0] - m_viewport[0][0]) /
             2 + m_viewport[0][0],

        (p[1] + 1) *
        (m_viewport[1][1] - m_viewport[0][1]) /
         2 + m_viewport[0][1],

    0);
}

bool Viewer::clipPlane(Point3D &a, Point3D &b, Point3D p, Vector3D normal) {
  double clipA = (a - p).dot(normal);
  double clipB = (b - p).dot(normal);

  if (clipA < 0 && clipB < 0) {
    return false;
  }
  if (!(clipA >= 0 && clipB >= 0)) {
    double t = clipA / (clipA - clipB);
    if (clipA < 0) {
      a = a + t * (b - a);
    }
    else {
      b = a + t * (b-a);
    }
  }

  return true;
}

Point3D Viewer::project(Point3D p) {
  Point3D t = m_projectionM * p;
  return Point3D(t[0]/ p[2], t[1]/ p[2], t[2]/ p[2]);
}


void Viewer::drawLine(Point3D a, Point3D b) {

  bool shouldDraw = true;
  // clip near and far planes
  shouldDraw &= clipPlane(a, b, Point3D(0, 0, m_near), Vector3D(0, 0, 1)); 
  shouldDraw &= clipPlane(a,b, Point3D(0,0, m_far), Vector3D(0, 0, -1));

  if (!shouldDraw) return;

  a = toViewport(project(a));
  b = toViewport(project(b));

  // clip to the viewport (top, right, bottom, left)
  shouldDraw &= clipPlane(a, b, Point3D(m_viewport[0][0], m_viewport[0][1], 0), Vector3D(0, 1, 0));
  shouldDraw &= clipPlane(a, b, Point3D(m_viewport[1][0], m_viewport[1][1], 0), Vector3D(-1, 0, 0));
  shouldDraw &= clipPlane(a, b, Point3D(m_viewport[1][0], m_viewport[1][1], 0), Vector3D(0, -1, 0));
  shouldDraw &= clipPlane(a, b, Point3D(m_viewport[0][0], m_viewport[0][1], 0), Vector3D(1, 0, 0));


  if (!shouldDraw) return;
  DEBUG_MSG("Draw Line " << a << " to " << b);
  draw_line(Point2D(a[0], a[1]), Point2D(b[0], b[1]));
}


bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;
  
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  DEBUG_MSG( "Stub: Button " << event->button << " pressed" );

  m_mouseLastPos = event->x;

  if (m_mode == VIEWPORT) {
    m_viewport[0] = Point2D(event->x, event->y);
  }

  invalidate();

  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  DEBUG_MSG("Stub: Button " << event->button << " released" );

  // normalize the viewport

  Point2D minView = Point2D(
    min(m_viewport[0][0], m_viewport[1][0]),
    min(m_viewport[0][1], m_viewport[1][1]));


  Point2D maxView = Point2D(
    max(m_viewport[0][0], m_viewport[1][0]),
    max(m_viewport[0][1], m_viewport[1][1]));
  m_viewport[0] = minView;
  m_viewport[1] = maxView;

  if (m_viewport[0][0] < 0) m_viewport[0][0] = 0;
  if (m_viewport[0][1] < 0) m_viewport[0][1] = 0;


  if (m_viewport[1][0] > get_width()) m_viewport[1][0] = get_width();
  if (m_viewport[1][1] > get_height()) m_viewport[1][1] = get_height();

  invalidate();

  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  DEBUG_MSG( "Stub: Motion at " << event->x << ", " << event->y);

  double dx = event->x - m_mouseLastPos;
  m_mouseLastPos = event->x;
  bool b1,b2,b3;
  b1 = event->state & GDK_BUTTON1_MASK;
  b2 = event->state & GDK_BUTTON2_MASK;

  b3 = event->state & GDK_BUTTON3_MASK;

  switch(m_mode) {
   case VIEW_R: {
      double rotateF = dx / 100.0;
      if (b1) {
        m_viewM = m_viewM * rotation(rotateF, 'x');
      }
      if (b2) {
        m_viewM = m_viewM * rotation(rotateF, 'y');
      }
      if (b3) {
        m_viewM = m_viewM * rotation(rotateF, 'z');
      }
      break;
    }
    case VIEW_T: {
      double translateF = dx * 0.05;
      if (b1) {
        m_viewM = m_viewM * translation(Vector3D(translateF, 0, 0));
      }
      if (b2) {
        m_viewM = m_viewM * translation(Vector3D(0, translateF, 0));
      }
      if (b3) {
        m_viewM = m_viewM * translation(Vector3D(0, 0, translateF));
      }

      break;
    }
    case VIEW_P: {
      if (b1) {
        m_fov += dx / 10.0;
        // limit fov
        if (m_fov < 5) m_fov = 5;
        if (m_fov > 160) m_fov = 160;
      }
      if (b2) {
        m_near += dx / 50;
      }
      if (b3) {
        m_far += dx / 50;
      }

      if (m_near < 0) m_near = 0;
      if (m_far < 0) m_far = 0;

      
      DEBUG_MSG("FOV: " << m_fov << " " << m_near << " " << m_far);
      break;
    }
    case MODEL_R: {
       double rotateF = dx / 100.0;
      if (b1) {
        m_modelM = m_modelM * rotation(rotateF, 'x');
      }
      if (b2) {
        m_modelM = m_modelM * rotation(rotateF, 'y');
      }
      if (b3) {
        m_modelM = m_modelM * rotation(rotateF, 'z');
      }
      break;
    }
    case MODEL_T: {
      double translateF = dx * 0.05;
      if (b1) {
        m_modelM = m_modelM * translation(Vector3D(translateF, 0, 0));
      }
      if (b2) {
        m_modelM = m_modelM * translation(Vector3D(0, translateF, 0));
      }
      if (b3) {
        m_modelM = m_modelM * translation(Vector3D(0, 0, translateF));
      }

      break;
    }
    case MODEL_S: {
       double scaleF = dx * 0.01;
      if (b1) {
        m_scalingM = m_scalingM * scaling(Vector3D(1.0 + scaleF, 1, 1));
      }
      if (b2) {
        m_scalingM = m_scalingM * scaling(Vector3D(1, 1.0 + scaleF, 1));
      }
      if (b3) {
        m_scalingM = m_scalingM * scaling(Vector3D(1, 1, 1.0 + scaleF));
      }

      break;
    }
    case VIEWPORT: {
      m_viewport[1] = Point2D(event->x, event->y);
      break;
    }
  }


  invalidate();
  

  return true;
}

void Viewer::update_status() {
  std::stringstream ss;
  ss << "Current Mode: ";

  switch(m_mode) {
    case VIEW_R:
      ss << "View Rotate";
      break;
    case VIEW_T:
      ss << "View Translate";
      break;
    case VIEW_P:
      ss << "View Perspective";
      break;
    case MODEL_R:
      ss << "Model Rotate";
      break;
    case MODEL_T:
      ss << "Model Translate";
      break;
    case MODEL_S:
      ss << "Model Scale";
      break;
    case VIEWPORT:
      ss << "Viewport";
      break;
  }

  ss << setprecision(4) << "\n FOV: " << m_fov << " Near: " << m_near << " Far: " << m_far;

  m_status->set_label(ss.str());
  
}
