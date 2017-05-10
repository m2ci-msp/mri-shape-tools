#ifndef __ADAPTIVE_SEARCH_H__
#define __ADAPTIVE_SEARCH_H__

#include <armadillo>

#include "mesh/Mesh.h"
#include "alignment/KdTree.h"
#include "neighborsearch/NormalPlaneSearch.h"

class AdaptiveSearch: public NormalPlaneSearch{

public:

  /*-------------------------------------------------------------------------*/

  AdaptiveSearch(
    Mesh& source,
    Mesh& target,
    KdTree*& kdTree,
    double& maxDist,
    double& maxAngle,
    double& searchRadius
    ) :
    NormalPlaneSearch(source, target, kdTree, maxDist, maxAngle),
    searchRadius(searchRadius) {
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

      int bestBelowIndex = -1;
      int bestAboveIndex = -1;

      if( find_candidates(sourceIndex, bestBelowIndex, bestAboveIndex) == true ) {

        sourceIndices.push_back(sourceIndex);

        if(bestBelowIndex != -1) {
          targetIndices.push_back(bestBelowIndex);
        }
        else{
          targetIndices.push_back(bestAboveIndex);
        }

      } // end if find_candidates

    } // end for

  }

  /*-------------------------------------------------------------------------*/

protected:

  /*-------------------------------------------------------------------------*/

  bool find_candidates(
    const int& sourceIndex,
    int& bestBelowIndex,
    int& bestAboveIndex
    ) const {

    bestBelowIndex = -1;
    bestAboveIndex = -1;

    double bestBelowDistance = this->searchRadius;
    double bestAboveDistance = this->searchRadius;

    const arma::vec& sourcePoint =
      this->source.get_vertices().at(sourceIndex);

    const arma::vec& sourceNormal =
      this->source.get_vertex_normals().at(sourceIndex);

    std::vector<int> candidates =
      this->kdTree->get_nearest_neighbors_index(
        sourcePoint, this->searchRadius
        );

    for(const int& targetIndex: candidates) {

      if(is_valid(sourceIndex, targetIndex)) {

        const arma::vec& targetPoint = this->target.get_vertices().at(targetIndex);

        const arma::vec connection = targetPoint - sourcePoint;
        const double distance = arma::norm(connection);

        const double angleRadians = arma::norm_dot(sourceNormal, connection);

        if(angleRadians > 0) {
          if(distance < bestAboveDistance){
            bestAboveIndex = targetIndex;
            bestAboveDistance = distance;
          } // end if distance
        } // end if angle
        else{
          if(distance < bestBelowDistance){
            bestBelowIndex = targetIndex;
            bestBelowDistance = distance;
          } // end if distance
        } // end else angle

      } // end if valid

    } // end for

    return (bestBelowIndex != -1 || bestAboveIndex != -1);

  }

  /*-------------------------------------------------------------------------*/

  /*
     inherited from BasicSearch:

     Mesh& source;
     Mesh& target;
     KdTree*& kdTree;
     double& maxDist;

     inherited from NormalPlaneSearch:

     double& maxAngle

  */

  double& searchRadius;

  /*-------------------------------------------------------------------------*/

};
#endif
