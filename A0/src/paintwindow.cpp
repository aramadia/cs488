#include "paintwindow.hpp"

PaintWindow::PaintWindow()
{
  set_title("488 Paint");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  
  // Set up the application menu
  // The slot we use here just causes PaintWindow::hide() on this,
  // which shuts down the application.
  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("Q"),
    sigc::mem_fun(*this, &PaintWindow::hide)));

  m_menu_app.items().push_back(MenuElem("_Clear",
    sigc::mem_fun(*this, &PaintWindow::clear)));
  
  // Set up the tools menu

  // We're going to be connecting a bunch of menu entries to the same
  // function. So, we put a slot corresponding to that function in
  // mode_slot.
  //
  // The type shows that this slot returns void (nothing, and takes
  // one argument, a PaintCanvas::Mode.
  sigc::slot1<void, PaintCanvas::Mode> mode_slot = 
  	sigc::mem_fun(m_canvas, &PaintCanvas::set_mode);

  // Now we set up the actual tools. SigC::bind takes a slot and makes
  // a new slot with one fewer parameter than the one passed to it,
  // and "binds in" the constant value passed.
  //
  // In our case we take the reference to PaintCanvas::set_mode we
  // declared above, and bind in the appropriate mode, making a slot
  // that calls set_mode with the given mode (line/oval/rectangle).

  m_menu_tools.items().push_back( MenuElem("_Line", 
  	sigc::bind( mode_slot, PaintCanvas::DRAW_LINE ) ) );
  m_menu_tools.items().push_back( MenuElem("_Oval", 
  	sigc::bind( mode_slot, PaintCanvas::DRAW_OVAL ) ) );
  m_menu_tools.items().push_back( MenuElem("_Rectangle", 
  	sigc::bind( mode_slot, PaintCanvas::DRAW_RECTANGLE ) ) );

  // Set up the help menu
  m_menu_help.items().push_back(MenuElem("_Line Help",
    sigc::mem_fun(*this, &PaintWindow::help_line)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Tools", m_menu_tools));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Help", m_menu_help));
  m_menubar.items().back().set_right_justified(true);
  
  // Pack in our widgets
  
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the canvas below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_canvas.set_size_request(300, 300);
  m_vbox.pack_start(m_canvas);


  m_quit_button.set_label("Quit");
  m_quit_button.signal_clicked().connect(sigc::mem_fun(*this, &PaintWindow::hide));
  m_vbox.pack_start(m_quit_button);

  show_all();
}

void PaintWindow::help_line()
{
  const char* message =
    "Drawing a Line\n"
    "\n"
    "To draw a line, press the left mouse button to mark the beginning of the line.  Drag the mouse to the end of the line and release the button.";
  
  Gtk::MessageDialog dialog(*this, message, true, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);

  dialog.run();
}
