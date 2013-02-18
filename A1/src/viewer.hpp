// Daniel Lam
// dalam
// 20297494

#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "game.hpp"

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

  enum DrawMode {
    WIRE, FACE, MULTI
  };
  // change draw mode
  void set_draw_mode(DrawMode mode);

  void toggle_double_buffer();

  Game &game() {return m_game; }

  void setTimer(int ms);

  void reset();
  void new_game();
  
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
  void drawCube(double x, double y, double z);
  Game m_game;
  bool tick();
  bool refresh();

  DrawMode m_mode;
  
  bool m_double_buffer_enabled;

  // current rotatioin offset
  double m_rotX, m_rotY, m_rotZ;
  double m_scale;


  // angular velocity
  double m_rotVX, m_rotVY, m_rotVZ;

  // x coordinate of the mouse position last process
  int m_mouseLastPos;



  // when non-zero, will reset the timer to the given value
  int m_resetTimer;

  // used to calculate the gravity persistence
  int m_last_angle;

};

#endif
