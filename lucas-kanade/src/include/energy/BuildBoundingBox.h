#ifndef __BUILD_BOUNDING_BOX_H__
#define __BUILD_BOUNDING_BOX_H__

#include <vector>
#include <cfloat>

#include <armadillo>

class BuildBoundingBox{

private:

  /*-------------------------------------------------------------------------*/

  arma::vec min;
  arma::vec max;

  /*-------------------------------------------------------------------------*/

public:

  /*-------------------------------------------------------------------------*/

  BuildBoundingBox(
                   const std::vector<arma::vec>& points
                   ) {

    double minX = DBL_MAX;
    double minY = DBL_MAX;
    double minZ = DBL_MAX;

    double maxX = - DBL_MAX;
    double maxY = - DBL_MAX;
    double maxZ = - DBL_MAX;

    for(const arma::vec& point: points) {

      minX = (minX < point(0))? minX : point(0);
      minY = (minY < point(1))? minY : point(1);
      minZ = (minZ < point(2))? minZ : point(2);

      maxX = (maxX > point(0))? maxX : point(0);
      maxY = (maxY > point(1))? maxY : point(1);
      maxZ = (maxZ > point(2))? maxZ : point(2);

    }

    this->min = arma::vec({minX, minY, minZ});
    this->max = arma::vec({maxX, maxY, maxZ});

  }

  /*-------------------------------------------------------------------------*/

  void get(arma::vec& min, arma::vec& max) const {

    min = this->min;
    max = this->max;

  }

  /*-------------------------------------------------------------------------*/

};
#endif
