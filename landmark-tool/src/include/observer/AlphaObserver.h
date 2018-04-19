#ifndef __ALPHA_OBSERVER_H__
#define __ALPHA_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "action/CreateInternalPlot.h"
#include "action/CleanDrawingArea.h"

class AlphaObserver {
  public:

    /*-----------------------------------------------------------------------*/

    AlphaObserver() {

      // connect handler to clicked signal
      auto builder = GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      Glib::RefPtr<Gtk::Adjustment> adjustment = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("segmentationAdjustment"));

      this->alphaChangedHandler = adjustment->signal_value_changed().connect(
          sigc::mem_fun(*this, &AlphaObserver::alpha_changed)
          );

    }


    /*-----------------------------------------------------------------------*/

    ~AlphaObserver() {
      this->alphaChangedHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void alpha_changed() {

      CleanDrawingArea clean;
      clean.execute();

      CreateInternalPlot action;
      action.execute();

    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection alphaChangedHandler;
};

#endif
