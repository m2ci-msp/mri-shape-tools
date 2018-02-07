#ifndef __UPDATE_ABSOLUTE_POSITION_LABEL_H__
#define __UPDATE_ABSOLUTE_POSITION_LABEL_H__

#include <sstream>

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/EventPosition.h"
#include "core/Point.h"

class UpdateAbsolutePositionLabel : public Action {

  public:
    /*----------------------------------------------------------------------*/

    UpdateAbsolutePositionLabel() {

      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->positionLabel = Glib::RefPtr<Gtk::Label>::cast_dynamic(builder->get_object("absolutePositionLabel"));

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // FIXME: Is this necessary?
      if( !EventPosition::get_instance()->is_valid() ) {
        // do nothing if the position is not valid
        return;
      }

      Point point = EventPosition::get_instance()->get(); 

      const Image& image = Data::get_instance()->get_current_image();
      const double originX = image.info().get_origin_x() / image.info().get_hx();
      const double originY = image.info().get_origin_y() / image.info().get_hy();
      const double originZ = image.info().get_origin_z() / image.info().get_hz();

      std::stringstream stream;

      stream << "( "
             << (int) point.get_canonical_x() + originX
             << ", "
             << (int) point.get_canonical_y() + originY
             << ", "
             << (int) point.get_canonical_z() + originZ
             << " )";

      this->positionLabel->set_text(stream.str());

    }

  /*----------------------------------------------------------------------*/

private:

  Glib::RefPtr<Gtk::Label> positionLabel;

};
#endif
