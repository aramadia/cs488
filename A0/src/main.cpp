#include <gtkmm.h>
#include "paintwindow.hpp"

int main(int argc, char** argv)
{
  // Construct our main loop
  Gtk::Main kit(argc, argv);

  // Construct our (only) window
  PaintWindow window;

  // And run the application!
  Gtk::Main::run( window );
}
