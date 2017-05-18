#ifndef __EVENT_POSITION_H__
#define __EVENT_POSITION_H__

#include <memory>
#include "singleton/Data.h"
#include "core/Point.h"

class EventPosition {
  public:

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<EventPosition> get_instance() {
      if(instance == nullptr) {
        instance = std::shared_ptr<EventPosition>(new EventPosition);
      }
      return instance;
    }

    /*----------------------------------------------------------------------*/

    void set(const Point& p) {
      this->position = p;
    }

    /*----------------------------------------------------------------------*/

    const Point& get() const {
      return this->position;
    }


    /*----------------------------------------------------------------------*/

    /** checks if the a position is contained in the image stack domain */
    bool is_valid() const {
      Point min( 0, 0, 0);
      auto max = Data::get_instance()->get_dimensions();

      return 
        ( this->position >= min ) &&
        ( this->position <= max );
    }



  private:
    EventPosition() {}

    /*----------------------------------------------------------------------*/

    Point position;
    static std::shared_ptr<EventPosition> instance;

};
#endif
