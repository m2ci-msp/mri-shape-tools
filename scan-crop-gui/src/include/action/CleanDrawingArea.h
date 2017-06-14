#ifndef __CLEAN_DRAWING_AREA_H__
#define __CLEAN_DRAWING_AREA_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"

class CleanDrawingArea : public Action {
  public:

    CleanDrawingArea() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get dimensions of drawing area
      builder->get_widget("mainDrawingArea", this->area);

    }

    virtual void execute() {

      Cairo::RefPtr<Cairo::Context> cr =
        this->area->get_window()->create_cairo_context();

      cr->save();
      cr->set_operator(Cairo::OPERATOR_SOURCE);
      cr->set_source_rgba(1, 1, 1, 1);
      cr->paint();
      cr->restore();

    }

    private:
      Gtk::DrawingArea* area;

};
#endif
