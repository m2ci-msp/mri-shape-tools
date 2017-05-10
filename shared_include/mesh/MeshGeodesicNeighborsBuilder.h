#ifndef __MESH_GEODESIC_NEIGHBORS_BUILDER_H__
#define __MESH_GEODESIC_NEIGHBORS_BUILDER_H__

#include <queue>
#include <set>

#include <armadillo>

#include "mesh/MeshNeighbors.h"
#include "mesh/MeshOneRingNeighborsBuilder.h"

class MeshGeodesicNeighborsBuilder{

  struct Path{
    int origin;
    std::set<int> previous;
    int current;
    double distance;
  };

public:

  /*-------------------------------------------------------------------------*/

  MeshGeodesicNeighborsBuilder(
    const Mesh& mesh, const double& maxDistance ) :
    mesh(mesh), maxDistance(maxDistance) {

    // get one-ring neighbors
    MeshOneRingNeighborsBuilder builder(mesh);
    this->oneRingNeighbors = builder.get_neighbors();

  }

  /*-------------------------------------------------------------------------*/

  MeshNeighbors& get_neighbors() {

    this->neighbors.clear();

    compute_geodesic_neighbors();

    this->neighbors.add_description("geodesic neighbors within search radius "
                                    + std::to_string(this->maxDistance));

    return this->neighbors;

  }
  /*-------------------------------------------------------------------------*/

private:

  /*-------------------------------------------------------------------------*/

  void compute_geodesic_neighbors() {

    const int vertexAmount = this->mesh.get_vertices().size();

    for( int i = 0; i < vertexAmount; ++i) {

      process_vertex(i);

    }

  }

  /*-------------------------------------------------------------------------*/



  void process_vertex( const int& i ) {

    Path path;
    path.origin = i;
    path.current = i;
    path.distance = 0;

    this->current.push(path);

    while( this->current.empty() == false ) {

      process_queue();

    }



  }

  /*-------------------------------------------------------------------------*/

  void process_queue() {

    while( this->current.empty() == false ) {

      Path path = this->current.front();
      process_neighbors(path);
      this->current.pop();

    }

  }

  /*-------------------------------------------------------------------------*/

  void process_neighbors( const Path& path ) {

    const std::set<int>& neighbors = this->oneRingNeighbors.get_neighbors(path.current);
    const std::vector<arma::vec> vertices = this->mesh.get_vertices();

    const arma::vec& vertex = vertices.at(path.current);

    for( const int& neighbor : neighbors ) {

      // skip the previous vertices
      if( path.previous.count(neighbor) != 0 ) { continue; }

      const arma::vec& vertexNeighbor = vertices.at(neighbor);

      const double distance = arma::norm(vertex - vertexNeighbor) + path.distance;

      if( distance < this->maxDistance ) {

        Path newPath = path;
        newPath.current = neighbor;
        newPath.distance = distance;
        newPath.previous.insert(path.current);

        this->current.push(newPath);
        this->neighbors.add_neighbor(path.origin, neighbor);

      } // end if

    } // end for

  } // end process_neighbors

  /*-------------------------------------------------------------------------*/

  const Mesh mesh;
  double maxDistance;

  MeshNeighbors neighbors;

  MeshNeighbors oneRingNeighbors;

  std::queue<Path> current;

  /*-------------------------------------------------------------------------*/

};

#endif
