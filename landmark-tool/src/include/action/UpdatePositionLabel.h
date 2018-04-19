#ifndef __UPDATE_POSITION_LABEL_H__
#define __UPDATE_POSITION_LABEL_H__

#include <sstream>

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/EventPosition.h"
#include "core/Point.h"

class UpdatePositionLabel : public Action {

  public:
    /*----------------------------------------------------------------------*/

    UpdatePositionLabel() {

      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->positionLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("positionLabel"));

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // FIXME: Is this necessary?
      if( !EventPosition::get_instance()->is_valid() ) {
        // do nothing if the position is not valid
        return;
      }

      Point point = EventPosition::get_instance()->get(); 

      std::stringstream stream;

      stream << "( "
             << (int) point.get_canonical_x()
             << ", "
             << (int) point.get_canonical_y()
             << ", "
             << (int) point.get_canonical_z()
             << " )";

      this->positionLabel->set_text(stream.str());

    }

  /*----------------------------------------------------------------------*/

private:

  Glib::RefPtr<Gtk::Label> positionLabel;

};
#endif
