#ifndef __SELECT_SLICE_H__
#define __SELECT_SLICE_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "core/ImageStack.h"
#include "singleton/Data.h"
#include "singleton/CoordinateSystem.h"
#include "core/Point.h"

/** Selects a slice by modifying the slice adjustment ui element */
class SelectSlice  : public Action {

  public:
    /*----------------------------------------------------------------------*/
    SelectSlice () {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      this->slices = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("sliceAdjustment"));


      this->index = 0;
    }
    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // do not configure anything if no stack is available
      if( !Data::get_instance()->stack_present()) {
        return;
      }

      if( this->slices->get_value() == this->index) {
        this->slices->value_changed();
      }
      else {
        this->slices->set_value(index);
      }

    }

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    Glib::RefPtr<Gtk::Adjustment> slices;

    int index;
};
#endif
