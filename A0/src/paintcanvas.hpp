#ifndef PAINTCANVAS_HPP
#define PAINTCANVAS_HPP

#include <list>

#include <gtkmm.h>

#include "algebra.hpp"

// Define a convenient alias to be able to draw graphics later.
// The Cairo graphics context class is a bit like the Graphics
// class in Java's AWT, so we'll borrow that name here.
//
typedef Cairo::RefPtr<Cairo::Context> Graphics;

// An abstract base class that defines shapes we'll store and
// draw.  Each shape is currently defined from two corners m_from 
// and m_to, and a method that asks the shape to draw itself.
// 
class PaintShape
{
public:
	virtual ~PaintShape() {}

	virtual void draw( Graphics gc ) = 0;

protected:
	PaintShape( const Point2D& from, const Point2D& to )
		: m_from( from )
		, m_to( to )
	{}

protected:
	Point2D		m_from;
	Point2D		m_to;
};

class PaintLine
	: public PaintShape
{
public:
	PaintLine( const Point2D& from, const Point2D& to )
		: PaintShape( from, to )
	{}

	virtual void draw( Graphics gc );
};

class PaintRect
	: public PaintShape
{
public:
	PaintRect( const Point2D& from, const Point2D& to )
		: PaintShape( from, to )
	{}

	virtual void draw( Graphics gc );
};

class PaintOval
	: public PaintShape
{
public:
	PaintOval( const Point2D& from, const Point2D& to )
		: PaintShape( from, to )
	{}

	virtual void draw( Graphics gc );
};

class PaintCanvas 
	: public Gtk::DrawingArea
{
public:
  enum Mode {
    DRAW_LINE,
    DRAW_OVAL,
    DRAW_RECTANGLE
  };

  PaintCanvas();
  virtual ~PaintCanvas();

  void set_mode(Mode mode) { m_mode = mode; }
  void clear();

protected:

  // Events we implement
  // Note that we could use gtkmm's "signals and slots" mechanism
  // instead, but for many classes there's a convenient member
  // function one just needs to define that'll be called with the
  // event.
  
  virtual bool on_button_press_event(GdkEventButton* button);
  virtual bool on_button_release_event(GdkEventButton* button);

  virtual bool on_expose_event( GdkEventExpose *event );

private:
  Mode m_mode; // what to do when a user clicks

  Point2D m_start_pos; // position where the user last clicked

  std::list<PaintShape*> m_shapes;
};

#endif
