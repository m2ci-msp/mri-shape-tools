#ifndef __CONFIGURE_SCROLLBARS_H__
#define __CONFIGURE_SCROLLBARS_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/InternalPlot.h"

/** action that configures the interface data of the scrollbars */
class ConfigureScrollbars : public Action {

  public:
    /*----------------------------------------------------------------------*/
    ConfigureScrollbars() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get adjustment for the two scrollbars
      this->hScroll = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("hScrollAdjustment"));
      this->vScroll = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("vScrollAdjustment"));

      // get internal plot
      this->plot = InternalPlot::get_instance()->get_plot();

      // get drawing area
      builder->get_widget("mainDrawingArea", this->area);
    }
    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // do not configure anything if no plot is available
      if( !this->plot ) {
        return;
      }


      int plotWidth, plotHeight;
      int areaWidth, areaHeight;


      get_plot_dimensions(plotWidth, plotHeight);

      get_drawing_area_dimensions(areaWidth, areaHeight);

      const int hUpper = plotWidth;
      const int vUpper = plotHeight;

      this->hScroll->set_upper(hUpper);
      this->vScroll->set_upper(vUpper);

      // set values to zero if entire plot fits on the drawing area
      if( areaWidth > plotWidth) {
        this->hScroll->set_value(0);
      }

      if( areaHeight > plotHeight) {
        this->vScroll->set_value(0);
      }

      this->hScroll->set_page_size( areaWidth );
      this->vScroll->set_page_size( areaHeight );

      // adjust values if they are too large
      if( this->vScroll->get_value() > vUpper - areaHeight) {
        this->vScroll->set_value(vUpper - areaHeight);
      }

      if( this->hScroll->get_value() > hUpper - areaWidth) {
        this->hScroll->set_value(hUpper - areaWidth);
      }



    }

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    void get_plot_dimensions(int& width, int& height) const {
      width = InternalPlot::get_instance()->get_width();
      height = InternalPlot::get_instance()->get_height();
    }

    /*----------------------------------------------------------------------*/

    void get_drawing_area_dimensions(int& width, int& height) const {

      width = this->area->get_allocated_width();
      height = this->area->get_allocated_height();

    }

    /*----------------------------------------------------------------------*/

    Glib::RefPtr<Gtk::Adjustment> hScroll;
    Glib::RefPtr<Gtk::Adjustment> vScroll;
    Gtk::DrawingArea* area;
    Cairo::RefPtr<Cairo::ImageSurface> plot;
};
#endif
