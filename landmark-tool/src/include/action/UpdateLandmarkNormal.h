#ifndef __UPDATE_LANDMARK_NORMAL_H__
#define __UPDATE_LANDMARK_NORMAL_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"
#include "singleton/EventPosition.h"
#include "core/Point.h"
#include "singleton/CoordinateSystem.h"

/* Update landmark normal */
class UpdateLandmarkNormal : public Action {

  public:
    /*----------------------------------------------------------------------*/

    UpdateLandmarkNormal() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      // FIXME: Is this necessary?
      if( !EventPosition::get_instance()->is_valid() ) {
        // do nothing if the position is not valid
        return;
      }

      auto mark = LandmarkPool::get_instance()->get_selected_landmark();

      // get landmark position 
      auto center = mark->get_position();
      // get event position
      auto outside = EventPosition::get_instance()->get(); 

      double nx, ny;

      // compute normal in circle
      compute_normal_circle( center, outside, nx, ny );

      // get current normal
      auto landmarkNormal = mark->get_normal();

      landmarkNormal.set_x(nx);
      landmarkNormal.set_y(ny);

      mark->set_normal(landmarkNormal);

    }

    private:

    /*----------------------------------------------------------------------*/

      void compute_normal_circle(
        const Point& center,
        const Point& outside,
        double& nx,
        double& ny
        ) {

        nx = outside.get_x() - center.get_x();
        ny = outside.get_y() - center.get_y();

        const double length = sqrt(pow(nx, 2) + pow(ny, 2));

        if( length > 1) {
          const double norm = 1. / length;
          nx *= norm;
          ny *= norm;
        }
        else {
          nx = 0;
          ny = 0;
        }
 
      }

    /*----------------------------------------------------------------------*/

};
#endif
