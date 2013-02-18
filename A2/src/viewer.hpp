#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "algebra.hpp"

#ifdef MY_DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

class AppWindow;

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
  Viewer();
  virtual ~Viewer();

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();

  // *** Fill in these functions (in viewer.cpp) ***
  enum InteractionMode {
    VIEW_R, VIEW_T, VIEW_P, MODEL_R, MODEL_T, MODEL_S, VIEWPORT
  };

  void set_interaction_mode(InteractionMode mode);

  // Set the parameters of the current perspective projection using
  // the semantics of gluPerspective().
  void set_perspective(double fov, double aspect,
                       double near, double far);

  // Restore all the transforms and perspective parameters to their
  // original state. Set the viewport to its initial size.
  void reset_view();

  void set_status(Gtk::Label * status) { m_status = status; }
  void set_window(AppWindow * window) { m_window = window; }

protected:

  // Events we implement
  // Note that we could use gtkmm's "signals and slots" mechanism
  // instead, but for many classes there's a convenient member
  // function one just needs to define that'll be called with the
  // event.

  // Called when GL is first initialized
  virtual void on_realize();
  // Called when our window needs to be redrawn
  virtual bool on_expose_event(GdkEventExpose* event);
  // Called when the window is resized
  virtual bool on_configure_event(GdkEventConfigure* event);
  // Called when a mouse button is pressed
  virtual bool on_button_press_event(GdkEventButton* event);
  // Called when a mouse button is released
  virtual bool on_button_release_event(GdkEventButton* event);
  // Called when the mouse moves
  virtual bool on_motion_notify_event(GdkEventMotion* event);

private:

  // 0 is min, 1 is max
  Point2D m_viewport[2];

  // perspective
  double m_fov;
  double m_near;
  double m_far;

  // original cube coordinates
  Point3D m_cube[8];
  Point3D m_gnomon[4];

  // translation
  Matrix4x4 m_modelM;
  Matrix4x4 m_viewM;
  Matrix4x4 m_projectionM;

  // separate matrix so model gnomon doesn't scale
  Matrix4x4 m_scalingM;

  int m_mouseLastPos;
  InteractionMode m_mode;

  void drawLine(Point3D a, Point3D b);
  void drawCube(Point3D cube[8]);
  void drawGnomon(Point3D gnomon[4]);

  Point3D toViewport(Point3D p );
  Point3D project(Point3D p);
  bool clipPlane(Point3D &a, Point3D &b, Point3D p, Vector3D normal);

  void update_status();
  AppWindow *m_window;
  Gtk::Label *m_status;
};

#endif
