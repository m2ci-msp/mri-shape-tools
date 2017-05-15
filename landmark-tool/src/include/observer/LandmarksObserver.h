#ifndef __LANDMARKS_OBSERVER_H__
#define __LANDMARKS_OBSERVER_H__

#include "singleton/LandmarkPool.h"
#include "action/SelectSlice.h"
#include "action/SelectTreeViewEntry.h"

class LandmarksObserver {
  public:

    /*-----------------------------------------------------------------------*/

    LandmarksObserver() {

      this->landmarkSelectedHandler =
        LandmarkPool::get_instance()->selected_landmark.connect(
            sigc::mem_fun(*this, &LandmarksObserver::landmark_selected)
            );

    }


    /*-----------------------------------------------------------------------*/

    ~LandmarksObserver() {
      this->landmarkSelectedHandler.disconnect();
    }

    /*-----------------------------------------------------------------------*/

    void landmark_selected() {
      SelectSlice action;
      action.execute();

      SelectTreeViewEntry select;
      select.execute();
    }

    /*-----------------------------------------------------------------------*/

  private:
    sigc::connection landmarkSelectedHandler;
};

#endif
