#ifndef __MESH_MEAN_DIFFERENCE_H__
#define __MESH_MEAN_DIFFERENCE_H__

#include <armadillo>

#include "mesh/Mesh.h"

class MeshMeanDifference{

public:

  static double compute(const Mesh& source, const Mesh& target) {

    const auto& sourcePoints = source.get_vertices();
    const auto& targetPoints = target.get_vertices();

    if(sourcePoints.size() != targetPoints.size()) {

      throw std::runtime_error("Meshes do not have same size.");

    }

    double total = 0;

    for(size_t i = 0; i < sourcePoints.size(); ++i) {

      const double distance =
        arma::norm(sourcePoints.at(i) - targetPoints.at(i));

      total += distance;

    }

    return total / sourcePoints.size();


  }
};

#endif
