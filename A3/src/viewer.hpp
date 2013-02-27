#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>

#include "algebra.hpp"




class SceneNode;

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:

  enum Mode {
    POSITION,
    JOINTS
  };

  enum Option {
    CIRCLE,
    ZBUFFER,
    BACK_CULL,
    FRONT_CULL
  };

  enum ResetType {
    RESET_POSITION,
    RESET_ORIENTATION,
    RESET_JOINTS,
    RESET_ALL
  };

  Viewer();
  virtual ~Viewer();

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();

  void reset(ResetType type);

  void undo();
  void redo();

  void setSceneRoot(SceneNode *root);

  // UI ELement control
  void setMode(Mode mode);
  void toggleOption(Option option);
  
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

  // Draw a circle for the trackball, with OpenGL commands.
  // Assumes the context for the viewer is active.
  void draw_trackball_circle();
  
private:

  // view stuff
  Matrix4x4 m_trackballTranslation;
  Matrix4x4 m_trackballRotation;

  Point2D m_lastMouse;

  SceneNode *m_root;

  Mode m_mode;
  bool m_drawCircle;

  bool m_frontCull;
  bool m_backCull;
  bool m_zBuffer;

  // selected nodes
  std::list<SceneNode*> m_selected;


  struct History {
    int id;
    double xRot, yRot;
  };

  typedef std::stack<History> HistoryStack;
  HistoryStack m_undo;
  HistoryStack m_redo;

};

#endif
