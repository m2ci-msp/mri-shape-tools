#ifndef __MESH_SMOOTH_H__
#define __MESH_SMOOTH_H__

#include <map>

#include "mesh/Mesh.h"
#include "mesh/MeshNeighbors.h"
#include "mesh/MeshOneRingNeighborsBuilder.h"

class MeshSmooth{


private:

  Mesh previous;
  Mesh& current;

  MeshNeighbors oneRing;

  std::vector<bool> isBoundary;

public:

  MeshSmooth(Mesh& mesh) : current(mesh) {

    find_boundary();

    build_neighbors();

  }

  void apply(const int& iterationAmount) {

    for(int i = 0; i < iterationAmount; ++i) {

      perform_smoothing();

    }

  }

private:

  void find_boundary() {

    const auto& faces = this->current.get_faces();

    std::vector<std::map <int, int> > edges(this->current.get_vertices().size());

    this->isBoundary.resize(this->current.get_vertices().size(), false);

    for( auto face: faces) {

      unsigned int first = face.back();
      unsigned int current = face.back();
      unsigned int previous = current;

      face.pop_back();

      do{

        current = face.back();
        face.pop_back();

        edges.at(current)[previous] += 1;
        edges.at(previous)[current] += 1;

        previous = current;

      }
      while(face.empty() == false);

      edges.at(current)[first] += 1;
      edges.at(first)[current] += 1;

    }

    for(size_t i = 0; i < edges.size(); ++i) {

      for(const auto& entry: edges.at(i)) {

        if(entry.second != 2) {

          this->isBoundary.at(i) = true;

        }

      }

    }

  }

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

      int neighborHoodSize = neighborSet.size();

      if(this->isBoundary.at(i) == false) {

        for(const int& neighborIndex: neighborSet) {

          newPosition += previousPositions.at(neighborIndex);

        }

      }
      else{

        neighborHoodSize = 0;

        for(const int& neighborIndex: neighborSet) {

          if(isBoundary.at(neighborIndex) == true ) {

            newPosition += previousPositions.at(neighborIndex);
            ++neighborHoodSize;

          }

        }
      }

      newPosition /= neighborHoodSize + 1.;

      currentPositions.at(i) = newPosition;

    }

  }

};
#endif
