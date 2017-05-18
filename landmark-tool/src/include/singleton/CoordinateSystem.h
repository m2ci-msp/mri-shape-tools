#ifndef __COORDINATE_SYSTEM_H__
#define __COORDINATE_SYSTEM_H__

#include <memory>
#include <vector>
#include <array>

enum class Axis : int {
  X = 0,
  Y = 1,
  Z
};

class CoordinateSystem {
  public:

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<CoordinateSystem> get_instance() {
      if(instance == nullptr) {
        instance = std::shared_ptr<CoordinateSystem>(new CoordinateSystem);
      }
      return instance;
    }

    /*----------------------------------------------------------------------*/

    void set_view_mode( int mode) {
      this->viewMode = mode;
    }

    /*----------------------------------------------------------------------*/

    int get_index(Axis axis) const {
      const int i = static_cast<int> ( axis );
      return this->indices.at(viewMode).at(i);
    }
    
    /*----------------------------------------------------------------------*/


  private:
    CoordinateSystem() {
      this->viewMode = 0;

      // coordinate access for XYZ
      this->indices.push_back( {{ 0, 1, 2 }} );
      // coordinate access for ZXY
      this->indices.push_back( {{ 2, 0, 1 }} );
      // coordinate access for ZYX
      this->indices.push_back( {{ 2, 1, 0 }} );

    }

    /*----------------------------------------------------------------------*/

    static std::shared_ptr<CoordinateSystem> instance;

    std::vector<std::array<int, 3> > indices; 

    int viewMode;


};
#endif
