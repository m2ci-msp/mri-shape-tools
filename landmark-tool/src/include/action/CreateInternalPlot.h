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

      this->segmentationAdjustment = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("segmentationAdjustment"));

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

      // check if segmentation mask is present
      if( Data::get_instance()->segmentation_stack_present()) {

        slice = Data::get_instance()->get_segmentation_slice(index);
        pattern = Cairo::SurfacePattern::create(slice);
        pattern->set_filter(Cairo::FILTER_NEAREST);
        cr->set_source(pattern);
        cr->paint_with_alpha(get_segmentation_alpha());

      }

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

      }

      cr->restore();
    }

    /*-----------------------------------------------------------------------*/

    void plot_position(const Point& position, Cairo::RefPtr<Cairo::Context> cr) {

      const double zoom = get_zoom_factor();
      // show at center of voxel
      const double x = ( position.get_x() + 0.5 ) * zoom;
      const double y = ( position.get_y() + 0.5 ) * zoom;

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

      // show at center of voxel
      const double x = ( point.get_x() + 0.5 ) * zoom;
      const double y = ( point.get_y() + 0.5 ) * zoom;

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

    double get_segmentation_alpha() const {

      return this->segmentationAdjustment->get_value();

    }

    /*-----------------------------------------------------------------------*/

    Cairo::RefPtr<Cairo::ImageSurface> plot;
    Glib::RefPtr<Gtk::Adjustment> sliceAdjustment;
    Glib::RefPtr<Gtk::Adjustment> zoomAdjustment;
    Glib::RefPtr<Gtk::Adjustment> segmentationAdjustment;

    /*-----------------------------------------------------------------------*/

};

#endif
