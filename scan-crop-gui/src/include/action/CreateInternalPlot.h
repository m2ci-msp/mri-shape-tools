#ifndef __CREATE_INTERNAL_PLOT_H__
#define __CREATE_INTERNAL_PLOT_H__

#include <array>

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/InternalPlot.h"
#include "singleton/Data.h"
#include "singleton/CoordinateSystem.h"
#include "core/ImageStack.h"
#include "core/Point.h"
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

      // get adjustments for cropping
      this->min = { {
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("minXAdjustment")),
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("minYAdjustment")),
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("minZAdjustment"))
        }
      };

      this->max = { { 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("maxXAdjustment")),
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("maxYAdjustment")),
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("maxZAdjustment"))
        }
      };

    }

    /*-----------------------------------------------------------------------*/

    virtual void execute() {
      if(!Data::get_instance()->stack_present()) {
        // do nothing if no stack is present
        return;
      }

      create_surface();

      InternalPlot::get_instance()->set_plot(this->plot);
    }

    /*-----------------------------------------------------------------------*/
  private:

    /*-----------------------------------------------------------------------*/

    int get_slice_index() const {
      return this->sliceAdjustment->get_value();
    }

    /*-----------------------------------------------------------------------*/

    void create_surface() {

      int index = get_slice_index();

      auto slice = Data::get_instance()->get_slice(index);

      // crop slice
      auto image = crop_slice(slice);

      double zoom = get_zoom_factor();

      this->plot =
        Cairo::ImageSurface::create(slice->get_format(),
            image->get_width() * zoom, image->get_height() * zoom);

      Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(this->plot);

      // save the current transformation matrix
      cr->save();

      // set scale
      cr->scale(zoom, zoom);

      // create a pattern from the image
      Cairo::RefPtr<Cairo::SurfacePattern> pattern = 
        Cairo::SurfacePattern::create(image);

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

    Cairo::RefPtr<Cairo::ImageSurface> crop_slice(
      Cairo::RefPtr<Cairo::ImageSurface> slice
    ) {

      const int minX = this->min.at(
          CoordinateSystem::get_instance()->get_index(Axis::X)
          )->get_value();
      const int minY = this->min.at(
          CoordinateSystem::get_instance()->get_index(Axis::Y)
          )->get_value();

      const int maxX = this->max.at(
          CoordinateSystem::get_instance()->get_index(Axis::X)
          )->get_value();
      const int maxY = this->max.at(
          CoordinateSystem::get_instance()->get_index(Axis::Y)
      )->get_value();

      const int width = maxX - minX;
      const int height = maxY - minY;

      Cairo::RefPtr<Cairo::ImageSurface> image = 
        Cairo::ImageSurface::create(
            slice->get_format(),
            width, height
            );

      Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(image);

      Cairo::RefPtr<Cairo::SurfacePattern> pattern = 
        Cairo::SurfacePattern::create(slice);

      Cairo::Matrix matrix =
        Cairo::translation_matrix(minX, minY);
      pattern->set_matrix(matrix);

      // save the current transformation matrix
      cr->save();


      cr->set_source(pattern);
      cr->rectangle(0, 0, width, height);

      cr->fill();

      // restore the old transformation matrix
      cr->restore();

      return image;
    }

    /*-----------------------------------------------------------------------*/

    double get_zoom_factor() const {
      return this->zoomAdjustment->get_value();

    }

    /*-----------------------------------------------------------------------*/

    std::array<Glib::RefPtr<Gtk::Adjustment>, 3> min;
    std::array<Glib::RefPtr<Gtk::Adjustment>, 3> max;

    Cairo::RefPtr<Cairo::ImageSurface> plot;
    Glib::RefPtr<Gtk::Adjustment> sliceAdjustment;
    Glib::RefPtr<Gtk::Adjustment> zoomAdjustment;
};

#endif
