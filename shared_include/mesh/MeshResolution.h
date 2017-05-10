#ifndef __MESH_RESOLUTION_H__
#define __MESH_RESOLUTION_H__

#include <vector>

#include <armadillo>

#include "mesh/Mesh.h"
#include "mesh/MeshOneRingNeighborsBuilder.h"

class MeshResolution{

public:

  /*-------------------------------------------------------------------------*/

  MeshResolution(const Mesh& mesh) : mesh(mesh) {

    MeshOneRingNeighborsBuilder builder(mesh);
    this->meshNeighbors = builder.get_neighbors();

    this->vertexAmount = mesh.get_vertices().size();

    this->edges = std::vector<bool> ( vertexAmount * vertexAmount, false );

    compute_resolution();

    this->edges.clear();

  }

  /*-------------------------------------------------------------------------*/

  double get_resolution() const {

    return this->resolution;

  }

  /*-------------------------------------------------------------------------*/

private:

  /*-------------------------------------------------------------------------*/

  void compute_resolution() {

   const auto& vertices = this->mesh.get_vertices();

    double distance = 0;
    int edgeAmount = 0;

    for( int vertexId = 0; vertexId < this->vertexAmount; ++vertexId) {

      std::set<int> oneRingNeighbors = this->meshNeighbors.get_neighbors(vertexId);

      for( const int& neighborId: oneRingNeighbors) {

        if( is_edge_already_processed(vertexId, neighborId) == false ) {

          mark_edge(vertexId, neighborId);

          ++edgeAmount;

          distance += arma::norm( vertices[vertexId] - vertices[neighborId] );

        } // end if

      } // end for neighborId

    } // end for vertexId

    this->resolution = distance / edgeAmount;

  }

  /*-------------------------------------------------------------------------*/

  bool is_edge_already_processed( const int& start, const int& end ) const {

    const int halfEdgeOne = start * this->vertexAmount + end;
    const int halfEdgeTwo = end * this->vertexAmount + start;

    return ( this->edges[halfEdgeOne] || this->edges[halfEdgeTwo] );

  }

  /*-------------------------------------------------------------------------*/

  void mark_edge(const int& start, const int& end) {

    const int index = start * this->vertexAmount + end;

    this->edges[index] = true;

  }

  /*-------------------------------------------------------------------------*/

  const Mesh& mesh;
  MeshNeighbors meshNeighbors;

  std::vector<bool> edges;

  int vertexAmount;

  double resolution = 0;

  /*-------------------------------------------------------------------------*/

};


#endif
