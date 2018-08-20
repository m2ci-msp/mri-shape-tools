#ifndef __PROJECT_MESH_H__
#define __PROJECT_MESH_H__

#include <stdexcept>

#include <armadillo>

#include "mesh/Mesh.h"
#include "image/Image.h"

class ProjectMesh{

public:

  ProjectMesh(const Mesh& mesh, Image& scan, int sampleAmount = 5) :
    mesh(mesh), scan(scan), sampleAmount(sampleAmount) {

    this->max = this->scan.extrema().get_max();

    if(this->max == this->scan.extrema().get_min() ) {

      this->max = this->max + 1;

    }

  }

  /*------------------------------------------------------------------------*/

  Image get_projected_mesh() {

    project_mesh_to_scan();
    return this->scan;

  }

  /*------------------------------------------------------------------------*/

private:

  /*------------------------------------------------------------------------*/

  void project_mesh_to_scan() {

    if( this->mesh.get_faces().size() > 0 ) {

      for( const auto& face: this->mesh.get_faces() ) {
        draw_face(face);
      }

    }
    else{

      for( const arma::vec& vertex: this->mesh.get_vertices() ) {

        draw_point(vertex);

      }

    }

  }

  /*------------------------------------------------------------------------*/

  void draw_face(const std::vector<unsigned int>& face) {

    // iterate over the edges between the vertices
    for( size_t i = 0; i < face.size() - 1; ++i) {

      arma::vec start = this->mesh.get_vertices().at(face.at(i    ));
      arma::vec end   = this->mesh.get_vertices().at(face.at(i + 1));

      draw_edge(start, end);

    }

    // create edge between starting vertex and end vertex 
    arma::vec start = this->mesh.get_vertices().at(face.at(0    ));
    arma::vec end   = this->mesh.get_vertices().at(face.at(face.size() - 1));

    draw_edge(start, end);

  }

  /*------------------------------------------------------------------------*/

  void draw_point(const arma::vec& point) {

    try{

      this->scan.access().at_coordinate(point(0), point(1), point(2) ) = this->max;

    }
    catch (std::out_of_range& exception) {

      std::cerr << "Warning: attempt to draw at invalid position." << std::endl;

    }

  }

  /*------------------------------------------------------------------------*/

  void draw_edge(arma::vec start, arma::vec end) {

    // compute normalized direction vector representing the edge
    const arma::vec direction = arma::normalise(end - start);

    // derive sample distance on edge
    const double sampleDistance =
      arma::norm(end - start) / this->sampleAmount;

    // sample edge
    for(int i = 0; i <= this->sampleAmount; ++i) {

      // sample point on edge
      arma::vec coordinate =
        start + i * sampleDistance * direction;

      draw_point(coordinate);

    } // end for

  } // end draw_edge

    /*------------------------------------------------------------------------*/

  Mesh mesh;
  Image scan;

  double max;
  double sampleAmount;

  /*------------------------------------------------------------------------*/

};
#endif
