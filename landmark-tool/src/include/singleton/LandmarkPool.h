#ifndef __LANDMARK_POOL_H__
#define __LANDMARK_POOL_H__

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <set>

#include "core/Landmark.h"
#include "core/Point.h"

/** A class that creates landmarks and keeps track of them. 
*/
class LandmarkPool {
  public:

    /*----------------------------------------------------------------------*/

    /** get unique instance of the class */
    static std::shared_ptr<LandmarkPool> get_instance() {
      // create instance if it does not exist yet
      if( instance == nullptr ) {
        instance = std::shared_ptr<LandmarkPool> (new LandmarkPool());
      }

      return instance;
    }

    /*----------------------------------------------------------------------*/

    /** standard Destructor */
    ~LandmarkPool(){
      for( auto elem : this->landmarks ) {
        // free storage
        delete elem.second;
      }
    }

    /*----------------------------------------------------------------------*/

    void clear() {
      // free storage
      for( auto elem : this->landmarks ) {
        // free storage
        delete elem.second;
      }

      // empty the maps
      this->landmarks.clear();
      this->landmarksInPlane[0].clear();
      this->landmarksInPlane[1].clear();
      this->landmarksInPlane[2].clear();

    }

    /*----------------------------------------------------------------------*/

    /** Creates a new landmark  */
    Landmark* create_landmark(
        const std::string& name, const Point& p) {

      // create a new node
      Landmark* newLandmark = new Landmark(name, p);

      const double x = p.get_canonical_x();
      const double y = p.get_canonical_y();
      const double z = p.get_canonical_z();


      this->landmarks[newLandmark->get_id()] = newLandmark;

      this->landmarksInPlane[0][(int) z].push_back(newLandmark);
      this->landmarksInPlane[1][(int) y].push_back(newLandmark);
      this->landmarksInPlane[2][(int) x].push_back(newLandmark);

      this->added_landmark.emit();

      return newLandmark;
    }

    /*----------------------------------------------------------------------*/

    /** Creates a new landmark  */
    Landmark* create_landmark(const Point& p) {

      // create a new node
      Landmark* newLandmark = new Landmark(p);

      const double x = p.get_canonical_x();
      const double y = p.get_canonical_y();
      const double z = p.get_canonical_z();


      this->landmarks[newLandmark->get_id()] = newLandmark;

      this->landmarksInPlane[0][(int) z].push_back(newLandmark);
      this->landmarksInPlane[1][(int) y].push_back(newLandmark);
      this->landmarksInPlane[2][(int) x].push_back(newLandmark);

      this->added_landmark.emit();

      return newLandmark;
    }

    /*----------------------------------------------------------------------*/

    void change_position(Landmark*& mark, const Point& p) {

      // get old coordinates
      const double x_old = mark->get_position().get_canonical_x();
      const double y_old = mark->get_position().get_canonical_y();
      const double z_old = mark->get_position().get_canonical_z();

      const double x_new = p.get_canonical_x();
      const double y_new = p.get_canonical_y();
      const double z_new = p.get_canonical_z();

      this->landmarksInPlane[0][(int) z_old].remove(mark);
      this->landmarksInPlane[1][(int) y_old].remove(mark);
      this->landmarksInPlane[2][(int) x_old].remove(mark);

      mark->set_position(p);

      this->landmarksInPlane[0][(int) z_new].push_back(mark);
      this->landmarksInPlane[1][(int) y_new].push_back(mark);
      this->landmarksInPlane[2][(int) x_new].push_back(mark);

    }

    /*----------------------------------------------------------------------*/

    void delete_landmark(Landmark*& mark) {
      const double x = mark->get_position().get_canonical_x();
      const double y = mark->get_position().get_canonical_y();
      const double z = mark->get_position().get_canonical_z();

      this->landmarksInPlane[0][(int) z].remove(mark);
      this->landmarksInPlane[1][(int) y].remove(mark);
      this->landmarksInPlane[2][(int) x].remove(mark);

      this->landmarks.erase(mark->get_id());

      delete mark;
    }

    /*----------------------------------------------------------------------*/


    /** Gets a landmark by Id. Returns nullptr if landmark does not exist.
     * @param[in] id The id of the landmark to retrieve.
     */
    Landmark* get_landmark_by_id(const int& id) const {

      // check if this landmark exists
      auto position = this->landmarks.find(id); 

      if( position != this->landmarks.cend() ) {
        return position->second;
      }

      return nullptr;
    }

    /*----------------------------------------------------------------------*/

    const std::list<Landmark*> & get_landmarks_for_slice( int index ) const {
      return this->landmarksInPlane.at(this->viewMode).at(index);
    }

    /*----------------------------------------------------------------------*/

    void set_view_mode( int mode ) {
      this->viewMode = mode;
    }

    /*----------------------------------------------------------------------*/

    const std::list<Landmark*> get_all_landmarks() const {
      std::list<Landmark*> result;

      for( const auto& elem: landmarks ) {
        result.push_back(elem.second);
      }

      return result;
    }

    /*----------------------------------------------------------------------*/

    void select_id(const int& id) {
      this->selectedId = id;
      this->selected_landmark.emit();
    }

    /*----------------------------------------------------------------------*/

    Landmark* get_selected_landmark() const {
      return get_landmark_by_id(this->selectedId);
    }
    /*----------------------------------------------------------------------*/


    sigc::signal<void> added_landmark;
    sigc::signal<void> selected_landmark;

    /*----------------------------------------------------------------------*/

  private:
    LandmarkPool() {
      viewMode = 0;
      selectedId = -1;
    } 

    /** Container of the already created nodes. */
    std::unordered_map<int, Landmark*> landmarks;
    std::unordered_map<int, std::unordered_map< int,
      std::list<Landmark*> > > landmarksInPlane;

    /** single active instance of the pool. */
    static std::shared_ptr<LandmarkPool> instance;

    double viewMode;

    int selectedId;

};

#endif
