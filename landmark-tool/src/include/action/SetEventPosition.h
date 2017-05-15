#ifndef __SET_EVENT_POSITION_H__
#define __SET_EVENT_POSITION_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/CoordinateSystem.h"
#include "singleton/PlotPosition.h"
#include "core/Point.h"

/** action that configures the interface data of the scrollbars */
class SetEventPosition : public Action {

  public:
    /*----------------------------------------------------------------------*/
    SetEventPosition(const double& x,
        const double& y) : pointerX(x), pointerY(y) {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      // get zoom factor 
      Glib::RefPtr<Gtk::Adjustment> zoomAdjustment = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("zoomAdjustment"));
      this->zoom = zoomAdjustment->get_value();

      // get slice number
      Glib::RefPtr<Gtk::Adjustment> sliceAdjustment = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("sliceAdjustment"));
      this->slice = sliceAdjustment->get_value();

      // get offsets
      Glib::RefPtr<Gtk::Adjustment> hScroll = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("hScrollAdjustment"));
      this->offsetX = hScroll->get_value();

      Glib::RefPtr<Gtk::Adjustment> vScroll = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("vScrollAdjustment"));
      this->offsetY = vScroll->get_value();


    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // remove the offset created by the shifted origin of the plot
      double org_x, org_y;
      PlotPosition::get_instance()->get_origin(org_x, org_y);
      this->pointerX -= org_x;
      this->pointerY -= org_y;

      // compute coordinates for current mode
      const double modeX =
        translate_and_scale(this->pointerX, this->offsetX, 1. / this->zoom);
      const double modeY =
        translate_and_scale(this->pointerY, this->offsetY, 1. / this->zoom);
      const double modeZ = this->slice;

      Point position( modeX, modeY, modeZ);

      // set position
      EventPosition::get_instance()->set(position);

    }

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    double translate_and_scale(const double& val,
        const double& trans, const double& scale) const {
      return ( val + trans ) * scale;
    }

    /*----------------------------------------------------------------------*/

    double pointerX;
    double pointerY;
    double offsetX;
    double offsetY;
    double zoom;
    double slice;

};
#endif
