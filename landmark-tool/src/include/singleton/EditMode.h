#ifndef __EDIT_MODE_H__
#define __EDIT_MODE_H__

#include <memory>

#include "singleton/GuiDescription.h"

enum class Mode : int {
  STANDARD,
  MOVE_LANDMARK,
  EDIT_NORMAL
};

class EditMode {
  public:

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<EditMode> get_instance() {
      if(instance == nullptr) {
        instance = std::shared_ptr<EditMode> (new EditMode);
      }
      return instance;
    }

    /*----------------------------------------------------------------------*/

    Mode get_mode() {
      return this->mode;
    }

    /*----------------------------------------------------------------------*/

    void set_mode(const Mode& mode) {
      this->mode = mode;
      update_status();
    }

    /*----------------------------------------------------------------------*/

  private:

    EditMode() {
      this->mode = Mode::STANDARD;

      // connect handler to clicked signal
      auto builder = GuiDescription::get_instance()->get_builder();
      Gtk::Window* window;

      builder->get_widget("mainWindow", window); 

      builder->get_widget("statusbar", this->statusBar);

      update_status();

    }

    /*----------------------------------------------------------------------*/

    void update_status() {
      this->statusBar->pop();

      switch(this->mode ) {
        case Mode::STANDARD:
          this->statusBar->push("Standard mode");
          break;

        case Mode::MOVE_LANDMARK:
          this->statusBar->push("Move mode");
          break;

        case Mode::EDIT_NORMAL:
          this->statusBar->push("Edit normal mode");
          break;
      }
    }

    /*----------------------------------------------------------------------*/

 
    Gtk::Statusbar* statusBar;
    static std::shared_ptr<EditMode> instance;
    Mode mode;

};
#endif
