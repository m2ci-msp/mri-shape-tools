#ifndef __NORMAL_PLANE_SEARCH_H__
#define __NORMAL_PLANE_SEARCH_H__

#include <armadillo>

#include "mesh/Mesh.h"
#include "alignment/KdTree.h"
#include "neighborsearch/BasicSearch.h"

class NormalPlaneSearch: public BasicSearch{

public:

  /*-------------------------------------------------------------------------*/

  NormalPlaneSearch(
    Mesh& source,
    Mesh& target,
    KdTree*& kdTree,
    double& maxDist,
    double& maxAngle
    ) :
    BasicSearch(source, target, kdTree, maxDist),
    maxAngle(maxAngle) {
  }

  /*-------------------------------------------------------------------------*/

protected:

  /*-------------------------------------------------------------------------*/

  virtual bool is_valid(
    const int& sourceIndex,
    const int& targetIndex) const {

    // get points
    const arma::vec& sourcePoint =
      this->source.get_vertices().at(sourceIndex);

    const arma::vec& targetPoint =
      this->target.get_vertices().at(targetIndex);

    // get normals
    const arma::vec& sourceNormal =
      this->source.get_vertex_normals().at(sourceIndex);

    const arma::vec& targetNormal =
      this->target.get_vertex_normals().at(targetIndex);

    const double distance = arma::norm(targetPoint - sourcePoint);

    // compute angle in degrees
    const double angle =
      acos(arma::norm_dot(sourceNormal, targetNormal)) / M_PI * 180;

    if(distance < this->maxDist && angle < this->maxAngle)  {
      return true;
    }
    else {
      return false;
    }

  }

  /*-------------------------------------------------------------------------*/

  /* inherited from BasicSearch:

  Mesh& source;
  Mesh& target;
  KdTree*& kdTree;
  double& maxDist;

  */

  double& maxAngle;

  /*-------------------------------------------------------------------------*/

};
#endif
