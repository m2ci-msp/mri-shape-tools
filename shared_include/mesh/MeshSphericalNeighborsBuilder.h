#ifndef __MESH_SPHERICAL_NEIGHBORS_BUILDER_H__
#define __MESH_SPHERICAL_NEIGHBORS_BUILDER_H__

#include "alignment/KdTree.h"
#include "mesh/MeshNeighbors.h"
#include "mesh/Mesh.h"

class MeshSphericalNeighborsBuilder{

public:

  /*-------------------------------------------------------------------------*/

  MeshSphericalNeighborsBuilder(
    const Mesh& mesh, const double& maxDistance) :
    mesh(mesh), maxDistance(maxDistance) {
  }

  /*-------------------------------------------------------------------------*/

  MeshNeighbors& get_neighbors() {

    compute_spherical_neighbors(this->maxDistance);

    this->neighbors.add_description("spherical neighbors within search radius "
                                    + std::to_string(this->maxDistance) );

    return this->neighbors;

  }


  /*-------------------------------------------------------------------------*/

private:

  /*-------------------------------------------------------------------------*/

  void compute_spherical_neighbors(const double& radius) {

    const auto& vertices = this->mesh.get_vertices();
    KdTree kdTree(vertices);

    for( size_t vertexId = 0; vertexId < vertices.size(); ++vertexId) {

      const arma::vec& vertex = vertices[vertexId];

      std::vector<int> neighbors = kdTree.get_nearest_neighbors_index(vertex, radius);

      for(const int& id: neighbors) {

        if( id == (int) vertexId ) {
          continue;
        }

        this->neighbors.add_neighbor(vertexId, id);

      } // end for neighbors

    } // end for vertices

  }

  /*-------------------------------------------------------------------------*/

  MeshNeighbors neighbors;

  /*-------------------------------------------------------------------------*/

  const Mesh mesh;

  /*-------------------------------------------------------------------------*/

  const double maxDistance;

  /*-------------------------------------------------------------------------*/

};

#endif
