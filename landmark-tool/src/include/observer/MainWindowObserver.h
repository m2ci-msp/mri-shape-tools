#ifndef __MAIN_WINDOW_OBSERVER_H__
#define __MAIN_WINDOW_OBSERVER_H__

#include <memory>

#include "singleton/GuiDescription.h"
#include "singleton/Data.h"
#include "singleton/EditMode.h"

class MainWindowObserver {

  public:

    /*-----------------------------------------------------------------------*/

    MainWindowObserver() : 
      mode(EditMode::get_instance()) {

      // connect handler to clicked signal
      auto builder = GuiDescription::get_instance()->get_builder();
      Gtk::Window* window;

      builder->get_widget("mainWindow", window); 

      this->keyPressHandler = window->signal_key_press_event().connect(
          sigc::mem_fun(*this, &MainWindowObserver::key_pressed)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~MainWindowObserver() {
      this->keyPressHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    bool key_pressed ( GdkEventKey* event) {

      if(!Data::get_instance()->stack_present()) {
        return false;
      }

      switch( event->keyval ) {
        case GDK_KEY_F1:
          toggle(Mode::MOVE_LANDMARK);
          break;

        default:
          break;

      }

      return false;
    }

    /*-----------------------------------------------------------------------*/

  private:

    void toggle(const Mode& mode) {
      if( this->mode->get_mode() == mode ) {
        this->mode->set_mode(Mode::STANDARD);
      }
      else {
        this->mode->set_mode(mode);
      }
    }

   /*-----------------------------------------------------------------------*/

    std::shared_ptr<EditMode> mode;
    sigc::connection keyPressHandler;
};

#endif
