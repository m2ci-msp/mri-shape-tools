#ifndef __SCROLLBAR_OBSERVER_H__
#define __SCROLLBAR_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "action/DisplayPlot.h"

class ScrollbarObserver {
  public:

    /*-----------------------------------------------------------------------*/

    ScrollbarObserver() {

      // connect handler to clicked signal
      auto builder = GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      Glib::RefPtr<Gtk::Adjustment> hScroll = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("hScrollAdjustment"));

      Glib::RefPtr<Gtk::Adjustment> vScroll = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("vScrollAdjustment"));

      this->hScrollHandler = hScroll->signal_value_changed().connect(
          sigc::mem_fun(*this, &ScrollbarObserver::scrolling_event)
          );

      this->vScrollHandler = vScroll->signal_value_changed().connect(
          sigc::mem_fun(*this, &ScrollbarObserver::scrolling_event)
          );

    }


    /*-----------------------------------------------------------------------*/

    ~ScrollbarObserver() {
      this->hScrollHandler.disconnect();
      this->vScrollHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void scrolling_event() {
      DisplayPlot action;
      action.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection hScrollHandler;
    sigc::connection vScrollHandler;
};

#endif
