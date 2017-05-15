#ifndef __FIND_NEAREST_LANDMARK_H__
#define __FIND_NEAREST_LANDMARK_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"
#include "singleton/EventPosition.h"
#include "core/Point.h"

/* Find nearest landmark next to the current event position */
class FindNearestLandmark : public Action {

  public:
    /*----------------------------------------------------------------------*/
    FindNearestLandmark() {
      // get builder object
      Glib::RefPtr<Gtk::Builder> builder =
        GuiDescription::get_instance()->get_builder();

      this->sliceAdjustment = 
        Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(
            builder->get_object("sliceAdjustment"));

      this->found = nullptr;


    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      if( !EventPosition::get_instance()->is_valid() ) {
        // do nothing if the position is not valid
        return;
      }

      auto point = EventPosition::get_instance()->get(); 
      const int index = get_slice_index();

      std::list<Landmark*> landmarks;

      try {
        landmarks = 
          LandmarkPool::get_instance()->get_landmarks_for_slice(index);
      }
      catch ( std::exception& e) {
      }

      double min = DBL_MAX;

      for( const auto& mark : landmarks ) {
        const double distance = mark->get_position().distance_to(point);
        if(  distance <  min ) {
          this->found = mark;
          min = distance;
        }
      }

    }

    /*----------------------------------------------------------------------*/

    Landmark* get_result() const {
      return this->found;
    }

    /*----------------------------------------------------------------------*/

  private:

    int get_slice_index() const {
      return this->sliceAdjustment->get_value();
    }


    Landmark* found;
    Glib::RefPtr<Gtk::Adjustment> sliceAdjustment;
};
#endif
