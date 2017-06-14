#ifndef __DRAWING_AREA_OBSERVER_H__
#define __DRAWING_AREA_OBSERVER_H__

#include "singleton/GuiDescription.h"
#include "singleton/EventPosition.h"
#include "singleton/Data.h"

#include "action/DisplayPlot.h"
#include "action/ConfigureScrollbars.h"
#include "action/SetEventPosition.h"
#include "action/CreateInternalPlot.h"
#include "action/ShowPosition.h"

class DrawingAreaObserver {
  public:

    /*-----------------------------------------------------------------------*/

    DrawingAreaObserver() {

      // connect handler to clicked signal
      auto builder = GuiDescription::get_instance()->get_builder();
      Gtk::DrawingArea* area;

      builder->get_widget("mainDrawingArea", area); 


      this->redrawHandler = area->signal_draw().connect(
          sigc::mem_fun(*this, &DrawingAreaObserver::redraw_triggered)
          );

      this->resizeHandler = area->signal_size_allocate().connect(
          sigc::mem_fun(*this, &DrawingAreaObserver::resize_triggered)
          );

      this->motionHandler = area->signal_motion_notify_event().connect(
          sigc::mem_fun(*this, &DrawingAreaObserver::motion_occured)
          );
    }


    /*-----------------------------------------------------------------------*/

    ~DrawingAreaObserver() {
      this->redrawHandler.disconnect();
      this->resizeHandler.disconnect();
      this->buttonPressHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    bool redraw_triggered(const Cairo::RefPtr<Cairo::Context>&) {
      DisplayPlot action;
      action.execute();
      return false;
    }

    /*-----------------------------------------------------------------------*/

    void resize_triggered( Gdk::Rectangle& ) {
      ConfigureScrollbars action;
      action.execute();
    }


    /*-----------------------------------------------------------------------*/

    bool motion_occured(GdkEventMotion *event) {

      // compute internal coordinates from cursor
      SetEventPosition setPosition(event->x, event->y);
      setPosition.execute();

      ShowPosition showPosition;
      showPosition.execute();
      
      return true;

    }

    /*-----------------------------------------------------------------------*/

  private:

    /*-----------------------------------------------------------------------*/

    sigc::connection redrawHandler;
    sigc::connection resizeHandler;
    sigc::connection buttonPressHandler;
    sigc::connection motionHandler;
};

#endif
