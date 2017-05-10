#ifndef __MESH_DISTANCE_H__
#define __MESH_DISTANCE_H__

#include <cfloat>

#include "mesh/Mesh.h"
#include "neighborsearch/NeighborSearch.h"

class MeshDistance{

public:

  /*--------------------------------------------------------------------------*/

  MeshDistance(const Mesh& mesh) : target(mesh) {

    this->search.set_target(target);
    this->search.set_max_distance(DBL_MAX);

  }

  /*--------------------------------------------------------------------------*/

  double compute(const Mesh& mesh) {

    this->search.set_source(mesh);

    std::vector<int> sourceIndices;
    std::vector<int> targetIndices;

    this->search.basic().find_neighbors(sourceIndices, targetIndices);

    double result = 0;

    for(unsigned int i = 0; i < sourceIndices.size(); ++i) {

      const int& sourceIndex = sourceIndices.at(i);
      const int& targetIndex = targetIndices.at(i);

      result +=
        arma::norm(mesh.get_vertices().at(sourceIndex) -
                   target.get_vertices().at(targetIndex)
          );
    }

    result /= sourceIndices.size();

    return result;

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  Mesh target;
  NeighborSearch search;

  /*--------------------------------------------------------------------------*/

};

#endif
