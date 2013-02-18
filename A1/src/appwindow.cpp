// Daniel Lam
// dalam
// 20297494

#include "appwindow.hpp"
#include <iostream>

#include "viewer.hpp"
#include "debug.hpp"

AppWindow::AppWindow()
{
  set_title("488 Tetrominoes on the Wall");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::CheckMenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem;

  
  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.

  m_menu_app.items().push_back(MenuElem("_New Game", Gtk::AccelKey("n"),
    sigc::mem_fun(m_viewer, &Viewer::new_game)));

  m_menu_app.items().push_back(MenuElem("_Reset", Gtk::AccelKey("r"),
    sigc::mem_fun(m_viewer, &Viewer::reset)));
  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
    sigc::mem_fun(*this, &AppWindow::hide)));
  


  sigc::slot1<void, Viewer::DrawMode> mode_slot = sigc::mem_fun(m_viewer, &Viewer::set_draw_mode);


  Gtk::RadioButtonGroup drawGroup;
  m_menu_draw.items().push_back(RadioMenuElem(drawGroup, "_Wire-frame", Gtk::AccelKey("w"), 
    sigc::bind(mode_slot, Viewer::WIRE)));

  m_menu_draw.items().push_back(RadioMenuElem(drawGroup, "_Face",Gtk::AccelKey("f"), 
    sigc::bind(mode_slot, Viewer::FACE)));
  m_menu_draw.items().push_back(RadioMenuElem(drawGroup, "_Multicoloured",Gtk::AccelKey("m"), 
    sigc::bind(mode_slot, Viewer::MULTI)));

  Gtk::RadioButtonGroup group;
  sigc::slot1<void, int> speed_slot = sigc::mem_fun(m_viewer, &Viewer::setTimer);
  m_menu_speed.items().push_back(RadioMenuElem(group, "Slow (_1)", Gtk::AccelKey("1"), 
    sigc::bind(speed_slot, 500)));
  m_menu_speed.items().push_back(RadioMenuElem(group, "Medium (_2)", Gtk::AccelKey("2"), 
    sigc::bind(speed_slot, 250)));
  m_menu_speed.items().push_back(RadioMenuElem(group, "Fast (_3)", Gtk::AccelKey("3"), 
    sigc::bind(speed_slot, 100)));







  m_menu_buffer.items().push_back(CheckMenuElem("Double _Buffer", Gtk::AccelKey("b"), 
    sigc::mem_fun(m_viewer, &Viewer::toggle_double_buffer)));

  // have the menu checked
  static_cast<Gtk::CheckMenuItem*>(&m_menu_buffer.items()[0])->set_active();

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Draw Mode", m_menu_draw));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Speed", m_menu_speed));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Buffer", m_menu_buffer));
  
  // Pack in our widgets
  
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 600);
  m_vbox.pack_start(m_viewer);

  show_all();
}

bool AppWindow::on_key_press_event( GdkEventKey *ev )
{
        // This is a convenient place to handle non-shortcut
        // keys.  You'll want to look at ev->keyval.


	// An example key; delete and replace with the
	// keys you want to process
  unsigned int key = ev->keyval;
  if (key == GDK_KEY_Left) {
    DEBUG_MSG("left");
    m_viewer.game().moveLeft();
    m_viewer.invalidate();
  }
  else if (key == GDK_KEY_Right) {
    DEBUG_MSG("right");
    m_viewer.game().moveRight();
    m_viewer.invalidate();
  }
  else if (key == GDK_KEY_Up) {
    DEBUG_MSG("GDK_KEY_Up");
    m_viewer.game().rotateCCW();
    m_viewer.invalidate();
  }
  else if (key == GDK_KEY_Down) {
    DEBUG_MSG("GDK_KEY_Down");
    m_viewer.game().rotateCW();
    m_viewer.invalidate();
  }
  else if (key == GDK_KEY_space) {
    DEBUG_MSG("GDK_KEY_Space");
    m_viewer.game().drop();
    m_viewer.invalidate();
  } else {
          return Gtk::Window::on_key_press_event( ev );
  }

  return true;
}
