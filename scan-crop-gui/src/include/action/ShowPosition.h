#ifndef __SHOW_POSITION_H__
#define __SHOW_POSITION_H__

#include <sstream>

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/EventPosition.h"

/** Creates a new selection region */
class ShowPosition : public Action {

  public:
    /*----------------------------------------------------------------------*/

    ShowPosition() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get status bar
      builder->get_widget("statusbar1", this->statusBar);

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      if( !EventPosition::get_instance()->is_valid() ) {
        // do nothing if the position is not valid
        return;
      }

      std::stringstream status;
      auto point = EventPosition::get_instance()->get();

      status << "( " << (int) point.get_canonical_x()
             << ", " << (int) point.get_canonical_y()
             << ", " << (int) point.get_canonical_z()
             << ")";

      this->statusBar->pop();
      this->statusBar->push(status.str());

    }

    /*----------------------------------------------------------------------*/

  private:
    Gtk::Statusbar* statusBar;
};
#endif
