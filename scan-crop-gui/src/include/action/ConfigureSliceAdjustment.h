#ifndef __CONFIGURE_SLICE_ADJUSTMENT_H__
#define __CONFIGURE_SLICE_ADJUSTMENT_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "core/ImageStack.h"
#include "singleton/Data.h"
#include "singleton/CoordinateSystem.h"
#include "core/Point.h"

/** action that configures the interface data of the slice adjustment */
class ConfigureSliceAdjustment : public Action {

  public:
    /*----------------------------------------------------------------------*/
    ConfigureSliceAdjustment() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      this->slices = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("sliceAdjustment"));

    }
    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // do not configure anything if no stack is available
      if( !Data::get_instance()->stack_present()) {
        return;
      }

      // set maximum number of slices
      this->slices->set_upper(get_slice_amount() - 1);
    }

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    int get_slice_amount() const {
      const auto dimensions = Data::get_instance()->get_dimensions();

      return dimensions.get_z();
    }

    /*----------------------------------------------------------------------*/

    Glib::RefPtr<Gtk::Adjustment> slices;
};
#endif
