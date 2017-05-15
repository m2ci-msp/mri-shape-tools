#ifndef __ZOOM_OBSERVER_H__
#define __ZOOM_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "action/CreateInternalPlot.h"
#include "action/CleanDrawingArea.h"

class ZoomObserver {
  public:

    /*-----------------------------------------------------------------------*/

    ZoomObserver() {

      // connect handler to clicked signal
      auto builder = GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      Glib::RefPtr<Gtk::Adjustment> slice = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("zoomAdjustment"));

      this->zoomChangedHandler = slice->signal_value_changed().connect(
          sigc::mem_fun(*this, &ZoomObserver::zoom_changed)
          );

    }


    /*-----------------------------------------------------------------------*/

    ~ZoomObserver() {
      this->zoomChangedHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void zoom_changed() {

      CleanDrawingArea clean;
      clean.execute();

      CreateInternalPlot action;
      action.execute();

    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection zoomChangedHandler;
};

#endif
