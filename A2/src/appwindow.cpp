#include "appwindow.hpp"

AppWindow::AppWindow()
{
  set_title("CS488 Assignment Two");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem;
  
  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.

  m_menu_app.items().push_back(MenuElem("_Reset", Gtk::AccelKey("A"),
    sigc::mem_fun(m_viewer, &Viewer::reset_view)));
  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
    sigc::mem_fun(*this, &AppWindow::hide)));
  


  Gtk::RadioButtonGroup modeGroup;

  sigc::slot1<void, Viewer::InteractionMode> mode_slot = sigc::mem_fun(m_viewer, &Viewer::set_interaction_mode);


  m_menu_mode.items().push_back(RadioMenuElem(modeGroup, "View R_otate", Gtk::AccelKey("O"), 
    sigc::bind(mode_slot, Viewer::VIEW_R)));
  m_menu_mode.items().push_back(RadioMenuElem(modeGroup, "View Tra_nslate", Gtk::AccelKey("N"), 
    sigc::bind(mode_slot, Viewer::VIEW_T)));
  m_menu_mode.items().push_back(RadioMenuElem(modeGroup, "View _Perspective", Gtk::AccelKey("P"), 
    sigc::bind(mode_slot, Viewer::VIEW_P)));


  m_menu_mode.items().push_back(RadioMenuElem(modeGroup, "Model _Rotate", Gtk::AccelKey("R"), 
    sigc::bind(mode_slot, Viewer::MODEL_R)));
  m_menu_mode.items().push_back(RadioMenuElem(modeGroup, "Model T_ranslate", Gtk::AccelKey("T"), 
    sigc::bind(mode_slot, Viewer::MODEL_T)));
  m_menu_mode.items().push_back(RadioMenuElem(modeGroup, "Model _Scale", Gtk::AccelKey("S"), 
    sigc::bind(mode_slot, Viewer::MODEL_S)));


  m_menu_mode.items().push_back(RadioMenuElem(modeGroup, "_Viewport", Gtk::AccelKey("V"), 
    sigc::bind(mode_slot, Viewer::VIEWPORT)));


  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Mode", m_menu_mode));
  


  // Pack in our widgets
  
  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 300);
  m_vbox.pack_start(m_viewer);

  m_status.set_size_request(300, 30);
  m_status.set_label("Temp");
  m_vbox.pack_start(m_status, Gtk::PACK_SHRINK);
  m_viewer.set_status(&m_status);

  m_viewer.set_window(this);
  set_mode(3);


  show_all();

  // Ensures that the viewport gets the size information
  m_viewer.reset_view();
}

void AppWindow::set_mode(int mode) {
  static_cast<Gtk::RadioMenuItem*>(&m_menu_mode.items()[mode])->set_active();
}
