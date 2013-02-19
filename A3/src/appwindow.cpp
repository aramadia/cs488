#include "appwindow.hpp"

AppWindow::AppWindow()
{
  set_title("Advanced Ergonomics Laboratory");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  
  using Gtk::Menu_Helpers::CheckMenuElem;
  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.

  // Application Menu

  m_menu_app.items().push_back(MenuElem("Reset Pos_ition", Gtk::AccelKey("i"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  m_menu_app.items().push_back(MenuElem("Reset _Orientation", Gtk::AccelKey("o"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  m_menu_app.items().push_back(MenuElem("Reset Joi_nts", Gtk::AccelKey("n"),
    sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_app.items().push_back(MenuElem("Reset _All", Gtk::AccelKey("a"),
    sigc::mem_fun(m_viewer, &Viewer::reset_all)));
  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  // Mode menu


  m_menu_mode.items().push_back(MenuElem("_Position/Orientation", Gtk::AccelKey("p"),
    sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_mode.items().push_back(MenuElem("_Joints", Gtk::AccelKey("j"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  // Edit menu
  m_menu_edit.items().push_back(MenuElem("_Undo", Gtk::AccelKey("u"),
    sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_edit.items().push_back(MenuElem("_Redot", Gtk::AccelKey("r"),
    sigc::mem_fun(*this, &AppWindow::hide)));

  // Options menu
  m_menu_option.items().push_back(CheckMenuElem("_Circle", Gtk::AccelKey("c"), 
    sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_option.items().push_back(CheckMenuElem("_Z-buffer", Gtk::AccelKey("z"), 
    sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_option.items().push_back(CheckMenuElem("_Backface cull", Gtk::AccelKey("b"), 
    sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_option.items().push_back(CheckMenuElem("_Frontface cull", Gtk::AccelKey("f"), 
    sigc::mem_fun(*this, &AppWindow::hide)));

  

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Application", m_menu_app));

  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Mode", m_menu_mode));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Edit", m_menu_edit));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("Options", m_menu_option));
  
  // Pack in our widgets
  
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 300);
  m_vbox.pack_start(m_viewer);

  show_all();
}
