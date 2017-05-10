#ifndef __MESH_ONE_RING_NEIGHBORS_BUILDER_H__
#define __MESH_ONE_RING_NEIGHBORS_BUILDER_H__

#include "mesh/MeshNeighbors.h"
#include "mesh/Mesh.h"

class MeshOneRingNeighborsBuilder{

public:

  /*-------------------------------------------------------------------------*/

  MeshOneRingNeighborsBuilder(const Mesh& mesh) : mesh(mesh) {
  }

  /*-------------------------------------------------------------------------*/

  MeshNeighbors& get_neighbors() {

    compute_one_ring_neighbors();

    this->neighbors.add_description("one-ring neighbors");

    return this->neighbors;

  }


  /*-------------------------------------------------------------------------*/

private:

  /*-------------------------------------------------------------------------*/

  void compute_one_ring_neighbors() {

    const auto& faces = this->mesh.get_faces();

    if( faces.size() == 0 ) {

      throw std::logic_error("No face information available - can not compute one-ring neighbors.");

    }

    for( const auto& face: faces ) {

      for( size_t index = 0; index < face.size(); ++index) {

        const int vertexId = face[index];

        const int leftNeighbor  = (index == 0)? face[face.size() - 1]: face[index - 1];

        const int rightNeighbor = (index == face.size() - 1)? face[0]: face[index + 1];

        // add to neighbors
        this->neighbors.add_neighbor(vertexId, leftNeighbor);
        this->neighbors.add_neighbor(vertexId, rightNeighbor);

      } // end for face

    } // end for faces

  } // end compute_one_ring_neighbors

  /*-------------------------------------------------------------------------*/

  MeshNeighbors neighbors;

  /*-------------------------------------------------------------------------*/

  const Mesh mesh;

  /*-------------------------------------------------------------------------*/

};

#endif
