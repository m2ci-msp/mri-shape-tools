#ifndef __SLICE_OBSERVER_H__
#define __SLICE_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "action/CreateInternalPlot.h"

class SliceObserver {
  public:

    /*-----------------------------------------------------------------------*/

    SliceObserver() {

      // connect handler to clicked signal
      auto builder = GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      Glib::RefPtr<Gtk::Adjustment> slice = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("sliceAdjustment"));

      this->sliceChangedHandler = slice->signal_value_changed().connect(
          sigc::mem_fun(*this, &SliceObserver::slice_changed)
          );

    }


    /*-----------------------------------------------------------------------*/

    ~SliceObserver() {
      this->sliceChangedHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void slice_changed() {
      CreateInternalPlot action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection sliceChangedHandler;
};

#endif
