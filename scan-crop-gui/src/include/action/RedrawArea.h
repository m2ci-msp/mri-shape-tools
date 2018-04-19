#ifndef __REDRAW_AREA_H__
#define __REDRAW_AREA_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

/** Redraw drawing area */
class RedrawArea : public Action {

public:

  /*-----------------------------------------------------------------------*/

  virtual void execute() {

    // get builder object
    Glib::RefPtr<Gtk::Builder> builder =
      GuiDescription::get_instance()->get_builder();

    // get dimensions of drawing area
    Gtk::DrawingArea* area;
    builder->get_widget("mainDrawingArea", area);

    area->queue_draw();

  }

  /*-----------------------------------------------------------------------*/


};

#endif
