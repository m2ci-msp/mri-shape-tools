#ifndef __LANDMARK_H__
#define __LANDMARK_H__

#include <string>

#include "ObjectWithId.h"
#include "core/Point.h"

/** A class representing a landmark on a 3D image.
 * A landmark consists of a 3D coordinate and a given name
 */
class Landmark: public ObjectWithId<Landmark> {

  public:

    /*----------------------------------------------------------------------*/

    /** Standard constructor */
    Landmark() : ObjectWithId<Landmark>() {
      // Use standard initializations

      // use NewLandmark as name
      this->name = std::string("NewLandmark");
    }

    /*----------------------------------------------------------------------*/

    /** Specialized constructor that initializes the object to the passed
     * values */
    Landmark(
        const std::string& name,
        const Point& p) :
      ObjectWithId<Landmark>(), position(p), name(name) {
      }

    /*----------------------------------------------------------------------*/

    Landmark(const Point& p) :
      ObjectWithId<Landmark>(), position(p) {
        this->name = std::string("Landmark" + std::to_string(this->id));
      }

    /*----------------------------------------------------------------------*/


    void set_position( const Point& p) {
      this->position = p;
    }

    /*----------------------------------------------------------------------*/

    void set_name( const std::string& name) {
      this->name = name;
    }

    /*----------------------------------------------------------------------*/

    const Point& get_position() const {
      return this->position;
    }

    /*----------------------------------------------------------------------*/

    const std::string& get_name() const {
      return this->name;
    }

    /*----------------------------------------------------------------------*/

  private:
    Point position;
    std::string name;

};
#endif
