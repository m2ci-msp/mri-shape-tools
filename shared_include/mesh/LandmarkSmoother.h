#ifndef __LANDMARK_SMOOTHER_H__
#define __LANDMARK_SMOOTHER_H__

#include <vector>
#include <set>

#include <armadillo>

#include "mesh/Mesh.h"
#include "mesh/MeshNeighbors.h"
#include "landmark/Landmark.h"

class LandmarkSmoother{

public:

  /*--------------------------------------------------------------------------*/

  LandmarkSmoother(
    const Mesh& mesh,
    const std::vector<Landmark> landmarks
    ) : mesh(mesh), landmarks(landmarks) {
  }

  /*--------------------------------------------------------------------------*/

  Mesh smooth() {

    MeshNeighbors meshNeighbors(this->mesh);

    return smooth_spikes(meshNeighbors);

  }

  /*--------------------------------------------------------------------------*/

  Mesh smooth(const double& radius) {

    MeshNeighbors meshNeighbors(this->mesh, radius, false);

    return smooth_spikes(meshNeighbors);

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  Mesh smooth_spikes(const MeshNeighbors& meshNeighbors) {

    Mesh copy(this->mesh);
    std::vector<arma::vec>& vertices = copy.get_vertices();

    for( const Landmark& mark : this->landmarks ) {

      const int& vertexIndex = mark.sourceIndex;
      const std::set<int> neighbors = meshNeighbors.get_neighbors(vertexIndex);

      arma::vec result = arma::zeros(3);

      for( const int& neighborIndex : neighbors ) {
        result += vertices[neighborIndex];
      }

      result  /= neighbors.size();

      vertices[vertexIndex] = result;


    }

    return copy;

  }

  /*--------------------------------------------------------------------------*/

  const Mesh& mesh;
  const std::vector<Landmark> landmarks;

  /*--------------------------------------------------------------------------*/

};

#endif
