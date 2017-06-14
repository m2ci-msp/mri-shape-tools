#ifndef __DISPLAY_PLOT_H__
#define __DISPLAY_PLOT_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/InternalPlot.h"
#include "singleton/PlotPosition.h"

/** Class that transfers the internal plot to the drawing area */
class DisplayPlot : public Action {

  public:

    /*-----------------------------------------------------------------------*/

    virtual void execute() {

      int offX, offY;

      double org_x, org_y;

      get_offsets(offX, offY);

      Gtk::DrawingArea* area = get_drawing_area();
      Cairo::RefPtr<Cairo::ImageSurface> plot = 
        InternalPlot::get_instance()->get_plot();

      if(!plot) {
        return;
      }
      // create a pattern from the image
      Cairo::RefPtr<Cairo::SurfacePattern> pattern = 
        Cairo::SurfacePattern::create(plot);

      this->area_width = area->get_allocated_width();
      this->area_height = area->get_allocated_height();

      this->plot_height = InternalPlot::get_instance()->get_height();
      this->plot_width = InternalPlot::get_instance()->get_width();

      // compute origin of plot in drawing area -> try to center it
      compute_origin(org_x, org_y);
      PlotPosition::get_instance()->set_origin(org_x, org_y);


      // translate to (offX - org_x, offY - org_y) -> consider origin
      // of plot and the selected area of the internal plot to be shown
      Cairo::Matrix matrix =
        Cairo::translation_matrix(offX - org_x, offY - org_y);
      pattern->set_matrix(matrix);

      Cairo::RefPtr<Cairo::Context> cr =
        area->get_window()->create_cairo_context();

      cr->set_source(pattern);
      cr->rectangle(org_x, org_y, plot_width, plot_height);
      cr->fill();

    }

    /*-----------------------------------------------------------------------*/

  private:

    void get_offsets( int& offX, int& offY) const {

      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      Glib::RefPtr<Gtk::Adjustment> hScroll = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("hScrollAdjustment"));

      Glib::RefPtr<Gtk::Adjustment> vScroll = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("vScrollAdjustment"));

      offX = hScroll->get_value();
      offY = vScroll->get_value();

    }

    /*-----------------------------------------------------------------------*/

    void compute_origin(double& x, double& y) const {
      x = 0.;
      y = 0.;

      if( area_width > plot_width ) {
        x = ( area_width - plot_width ) / 2.;
      }

      if( area_height > plot_height ) {
        y = ( area_height - plot_height ) / 2.;
      }
    }

    /*-----------------------------------------------------------------------*/

    Gtk::DrawingArea* get_drawing_area() const {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get dimensions of drawing area
      Gtk::DrawingArea* area;
      builder->get_widget("mainDrawingArea", area);

      return area;

    }


    /*-----------------------------------------------------------------------*/

    int area_width;
    int area_height;

    int plot_width;
    int plot_height;
};

#endif
