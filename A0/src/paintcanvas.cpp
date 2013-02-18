#include "paintcanvas.hpp"

using namespace std;

// In the three methods below, we use Cairo's drawing commands to
// build lines, rectangles, and ovals.  Of course, you can play with
// Cairo to create all kinds of additional shapes, images, text, and
// so on.  Once you've got a Cairo graphics context, you can use the
// documentation here:
//    http://www.cairographics.org/documentation/cairomm/reference/
// To look at other drawing commands.
//

void PaintLine::draw( Graphics gc ) 
{
	gc->move_to( m_from[0], m_from[1] );
	gc->line_to( m_to[0], m_to[1] );
	gc->stroke();
}

void PaintRect::draw( Graphics gc ) 
{
	gc->move_to( m_from[0], m_from[1] );
	gc->line_to( m_to[0], m_from[1] );
	gc->line_to( m_to[0], m_to[1] );
	gc->line_to( m_from[0], m_to[1] );
	gc->close_path();
	gc->fill();
}

void PaintOval::draw( Graphics gc ) 
{
	double width = fabs( m_from[0] - m_to[0] );
	double height = fabs( m_from[1] - m_to[1] );
	double xc = 0.5 * ( m_from[0] + m_to[0] );
	double yc = 0.5 * ( m_from[1] + m_to[1] );

	// Cairo knows how to draw circular arcs, so we'll embed the
	// arc inside a non-uniform scale.
	//
	gc->save();
		gc->translate( xc, yc );
		gc->scale( width*0.5, height*0.5 );
		gc->arc( 0.0, 0.0, 1.0, 0.0, 2.0*M_PI );
		gc->fill();
	gc->restore();
}

PaintCanvas::PaintCanvas()
  : Gtk::DrawingArea()
  , m_mode(DRAW_LINE)
{
	// The DrawingArea class needs to be told that it ought to
	// receive button presses and releases.  If you want the
	// canvas to receive other events, you need to include them here.
	//
	add_events( Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
}

PaintCanvas::~PaintCanvas()
{
	// Some basic memory management: free up the memory associated
	// with all the stored shapes.
	//
	for( list<PaintShape*>::iterator i = m_shapes.begin(); 
			i != m_shapes.end(); ++i ) {
		delete *i;
	}
}

bool PaintCanvas::on_button_press_event(GdkEventButton* button)
{
  if (button->button == 1) { // Left mouse button
    m_start_pos = Point2D( button->x, button->y );
  }
  
  return true;
}

bool PaintCanvas::on_button_release_event(GdkEventButton* button)
{
  if (button->button == 1) { // Left mouse button
  	Point2D to( button->x, button->y );

	PaintShape *shape = NULL;

    switch (m_mode) {
    case DRAW_LINE:
	  shape = new PaintLine( m_start_pos, to );
      break;
    case DRAW_OVAL:
	  shape = new PaintOval( m_start_pos, to );
      break;
    case DRAW_RECTANGLE:
	  shape = new PaintRect( m_start_pos, to );
      break;
    default:
      break;
    }

    if (shape) {
		m_shapes.push_back( shape );
		on_expose_event( NULL );
    }
  }
  
  return true;
}

bool PaintCanvas::on_expose_event( GdkEventExpose *event )
{
	Graphics gc = get_window()->create_cairo_context();

	// Clear the canvas to white.
	//
	gc->set_source_rgb( 1.0, 1.0, 1.0 );
	gc->paint();

	// Set the colour back to black for all the primitives.
	//
	gc->set_source_rgb( 0.0, 0.0, 0.0 );

	// Iterate over the stored shapes and ask them to draw themselves.
	//
	for( list<PaintShape*>::iterator i = m_shapes.begin(); 
			i != m_shapes.end(); ++i ) {
		PaintShape *shape = (*i);
		shape->draw( gc );
	}

	// Tell the superclass that expose happened.  Probably not important,
	// but do it just in case.
	//
	return Gtk::DrawingArea::on_expose_event( event );
}
