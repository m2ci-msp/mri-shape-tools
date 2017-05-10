#ifndef __MESH_AVERAGER_H__
#define __MESH_AVERAGER_H__

#include <vector>
#include <armadillo>

#include "mesh/Mesh.h"

class MeshAverager{

public:

  static Mesh average(const std::vector<Mesh>& meshes) {

    // averaged vertex data
    std::vector<arma::vec> averagedVertices;

    // first mesh in collection
    const Mesh& first = meshes.at(0);

    // get amount of vertices
    const size_t vertexAmount = meshes.at(0).get_vertices().size();

    // iterate over vertices
    for(size_t v = 0; v < vertexAmount; ++v) {

      arma::vec vertex({0, 0, 0});

      // iterate over meshes
      for(const Mesh& mesh: meshes) {

        vertex += mesh.get_vertices().at(v);

      }

      // average vertex data
      vertex /= meshes.size();

      averagedVertices.push_back(vertex);
    }

    Mesh result;

    result.set_vertices(averagedVertices) \
      .set_faces(first.get_faces());

    return result;

  }

};
#endif
