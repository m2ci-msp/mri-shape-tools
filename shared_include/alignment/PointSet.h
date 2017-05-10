#ifndef __POINT_SET_H__
#define __POINT_SET_H__

#include <vector>
#include <armadillo>

class PointSet{

  public:

    /*-----------------------------------------------------------------------*/

    static std::vector<arma::vec> unserialize(
        const std::vector<double>& points
        ) {

      std::vector<arma::vec> converted;

      const size_t amount = points.size() / 3;

      for(size_t i = 0; i < amount; ++i) {

        // compute index of current point
        const size_t index = 3 * i;

        const double x = points.at(index + 0);
        const double y = points.at(index + 1);
        const double z = points.at(index + 2);

        converted.push_back(arma::vec({x, y, z}));

      }

      return converted;
    }

    /*-----------------------------------------------------------------------*/

    static std::vector<double> serialize(
        const std::vector<arma::vec>& points
        ) {

      std::vector<double> converted;

      for( const auto& point: points) {
        
        converted.push_back(point(0));
        converted.push_back(point(1));
        converted.push_back(point(2));

        }

      return converted;
    }

    /*-----------------------------------------------------------------------*/
};

#endif
