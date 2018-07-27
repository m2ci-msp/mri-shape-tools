#ifndef __MESH_SMOOTH_H__
#define __MESH_SMOOTH_H__

#include "mesh/Mesh.h"
#include "mesh/MeshNeighbors.h"
#include "mesh/MeshOneRingNeighborsBuilder.h"

class MeshSmooth{


private:

  Mesh previous;
  Mesh& current;

  MeshNeighbors oneRing;

public:

  MeshSmooth(Mesh& mesh) : current(mesh) {

    build_neighbors();

  }

  void apply(const int& iterationAmount) {

    for(int i = 0; i < iterationAmount; ++i) {

      perform_smoothing();

    }

  }

private:

  void build_neighbors() {

    this->oneRing = MeshOneRingNeighborsBuilder(this->current).get_neighbors();

  }

  void perform_smoothing() {

    this->previous = this->current;

    std::vector<arma::vec>& currentPositions = this->current.get_vertices();
    const std::vector<arma::vec>& previousPositions = this->previous.get_vertices();

    for(size_t i = 0; i < previousPositions.size(); ++i) {

      arma::vec newPosition = previousPositions.at(i);

      const std::set<int> neighborSet = this->oneRing.get_neighbors(i);

      for(const int& neighborIndex: neighborSet) {

        newPosition += previousPositions.at(neighborIndex);

      }

      newPosition /= neighborSet.size() + 1.;

      currentPositions.at(i) = newPosition;

    }

  }

};
#endif
