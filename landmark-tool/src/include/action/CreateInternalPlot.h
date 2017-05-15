#ifndef __CREATE_INTERNAL_PLOT_H__
#define __CREATE_INTERNAL_PLOT_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/InternalPlot.h"
#include "singleton/Data.h"
#include "core/ImageStack.h"
#include "core/Point.h"
#include "singleton/LandmarkPool.h"
#include "singleton/CoordinateSystem.h"


/** Class that updates the internal current plot */
class CreateInternalPlot : public Action {

  public:

    /*-----------------------------------------------------------------------*/

    CreateInternalPlot() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get zoom adjustment 
      this->zoomAdjustment = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("zoomAdjustment"));

      this->sliceAdjustment = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("sliceAdjustment"));
    }

    /*-----------------------------------------------------------------------*/

    virtual void execute() {
      if(!Data::get_instance()->stack_present()) {
        // do nothing if no stack is present
        return;
      }

      create_surface();
      plot_landmarks();
      highlight_selected_landmark();

      InternalPlot::get_instance()->set_plot(this->plot);
    }

    /*-----------------------------------------------------------------------*/
  private:

    const std::list<Landmark*> get_landmarks() const {

      const int index = get_slice_index();
      return LandmarkPool::get_instance()->get_landmarks_for_slice(index);

    }

    /*-----------------------------------------------------------------------*/

    int get_slice_index() const {
      return this->sliceAdjustment->get_value();
    }

    /*-----------------------------------------------------------------------*/

    void create_surface() {

      int index = get_slice_index();

      auto slice = Data::get_instance()->get_slice(index);
      double zoom = get_zoom_factor();


      this->plot =
        Cairo::ImageSurface::create(slice->get_format(),
            slice->get_width() * zoom, slice->get_height() * zoom);

      Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(this->plot);

      // save the current transformation matrix
      cr->save();

      // set scale
      cr->scale(zoom, zoom);

      // create a pattern from the image
      Cairo::RefPtr<Cairo::SurfacePattern> pattern = 
        Cairo::SurfacePattern::create(slice);

      // set filter to nearest neighbor to avoid blurring
      pattern->set_filter(Cairo::FILTER_NEAREST);

      // set source pattern
      cr->set_source(pattern);
      // plot the entire pattern on the widget
      cr->paint();

      // restore the old transformation matrix
      cr->restore();


    }

    /*-----------------------------------------------------------------------*/

    void plot_landmarks() {
      std::list<Landmark*> landmarks;

      try {
        landmarks = get_landmarks();
      }
      catch ( std::exception& ) {
      }


      Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(this->plot);

      // save the current transformation matrix
      cr->save();


      for( auto mark : landmarks ) {
        
        auto position = mark->get_position();

        plot_position(position, cr);

        auto normal = mark->get_normal();

        plot_normal(position, normal, cr);

      }

      cr->restore();
    }

    /*-----------------------------------------------------------------------*/

    void plot_position(const Point& position, Cairo::RefPtr<Cairo::Context> cr) {

      const double zoom = get_zoom_factor();
      const double x = position.get_x() * zoom; 
      const double y = position.get_y() * zoom; 

      cr->set_line_width(2.);
      cr->set_source_rgb(1, 0, 0);
      cr->move_to( x - 6, y);
      cr->line_to( x + 6, y);
      cr->stroke();
      cr->move_to( x, y - 6);
      cr->line_to( x, y + 6);
      cr->stroke();

    }

    /*-----------------------------------------------------------------------*/

    void plot_normal(
      const Point& position,
      const Point& normal,
      Cairo::RefPtr<Cairo::Context> cr) {

      const double zoom = get_zoom_factor();

      const double sourceX = position.get_x() * zoom; 
      const double sourceY = position.get_y() * zoom; 

      const double targetX = ( position.get_x() + 5 * normal.get_x() ) * zoom; 
      const double targetY = ( position.get_y() + 5 * normal.get_y() ) * zoom; 

      cr->set_line_width(2.);
      cr->set_source_rgb(1, 1, 0);
      cr->move_to( sourceX, sourceY);
      cr->line_to( targetX, targetY);
      cr->stroke();

    }

    /*-----------------------------------------------------------------------*/

    void highlight_selected_landmark() {
      const int index = get_slice_index();

      auto mark = LandmarkPool::get_instance()->get_selected_landmark();

      if( mark == nullptr) {
        return;
      }

      auto point = mark->get_position();

      if( (int) point.get_z() != index ) {
        return;
      }

      Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(this->plot);

      // save the current transformation matrix
      cr->save();

      cr->set_line_width(2.);
      cr->set_source_rgb(0, 1, 0);

      const double zoom = get_zoom_factor();

      const double x = point.get_x() * zoom; 
      const double y = point.get_y() * zoom; 

      cr->move_to( x - 6, y);
      cr->line_to( x + 6, y);
      cr->stroke();
      cr->move_to( x, y - 6);
      cr->line_to( x, y + 6);
      cr->stroke();

      cr->restore();



    }
    /*-----------------------------------------------------------------------*/

    double get_zoom_factor() const {
      return this->zoomAdjustment->get_value();

    }

    /*-----------------------------------------------------------------------*/
    Cairo::RefPtr<Cairo::ImageSurface> plot;
    Glib::RefPtr<Gtk::Adjustment> sliceAdjustment;
    Glib::RefPtr<Gtk::Adjustment> zoomAdjustment;
};

#endif
