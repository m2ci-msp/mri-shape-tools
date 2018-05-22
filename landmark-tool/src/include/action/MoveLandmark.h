#ifndef __MOVE_LANDMARK_H__
#define __MOVE_LANDMARK_H__

#include "action/Action.h"
#include "singleton/GuiDescription.h"
#include "singleton/LandmarkPool.h"
#include "singleton/EventPosition.h"
#include "core/Point.h"

/** Moves the currently selected landmark to the event position */
class MoveLandmark: public Action {

  public:
    /*----------------------------------------------------------------------*/
    MoveLandmark() {

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      if( !EventPosition::get_instance()->is_valid() ) {
        // do nothing if the position is not valid
        return;
      }

      auto point = EventPosition::get_instance()->get(); 

      // convert point to voxel containing it
      point.set_x( (int) point.get_x() );
      point.set_y( (int) point.get_y() );
      point.set_z( (int) point.get_z() );

      auto mark = LandmarkPool::get_instance()->get_selected_landmark(); 

      LandmarkPool::get_instance()->change_position(mark, point);

    }

    /*----------------------------------------------------------------------*/

};
#endif
