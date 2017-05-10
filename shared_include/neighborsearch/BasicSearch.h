#ifndef __BASIC_SEARCH_H__
#define __BASIC_SEARCH_H__

#include <armadillo>

#include "mesh/Mesh.h"
#include "alignment/KdTree.h"

#include "SearchProto.h"

class BasicSearch : public SearchProto {

public:

  /*-------------------------------------------------------------------------*/

  BasicSearch(
    Mesh& source,
    Mesh& target,
    KdTree*& kdTree,
    double& maxDist
    ) :
    source(source),
    target(target),
    kdTree(kdTree),
    maxDist(maxDist) {
  }

  /*-------------------------------------------------------------------------*/

  virtual void find_neighbors(
    std::vector<int>& sourceIndices,
    std::vector<int>& targetIndices
    ) const {

    sourceIndices.clear();
    targetIndices.clear();

    const std::vector<arma::vec>& sourcePoints =
      this->source.get_vertices();

    for(unsigned int sourceIndex = 0;
        sourceIndex < sourcePoints.size(); ++sourceIndex) {

      const int targetIndex =
        this->kdTree->get_nearest_neighbor_index(
          sourcePoints.at(sourceIndex)
          );

      if( is_valid(sourceIndex, targetIndex) == true ) {
        sourceIndices.push_back(sourceIndex);
        targetIndices.push_back(targetIndex);
      }

    }

  }

  /*-------------------------------------------------------------------------*/

protected:

  /*-------------------------------------------------------------------------*/

  virtual bool is_valid(
    const int& sourceIndex,
    const int& targetIndex) const {

    const arma::vec& sourcePoint =
      this->source.get_vertices().at(sourceIndex);
    const arma::vec& targetPoint =
      this->target.get_vertices().at(targetIndex);

    if( arma::norm(targetPoint - sourcePoint) < this->maxDist)  {
      return true;
    }
    else {
      return false;
    }

  }

  /*-------------------------------------------------------------------------*/

  Mesh& source;
  Mesh& target;
  KdTree*& kdTree;
  double& maxDist;

  /*-------------------------------------------------------------------------*/

};
#endif
