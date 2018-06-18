#ifndef __MESH_BUILDER_H__
#define __MESH_BUILDER_H__

#include <vector>
#include <stdexcept>

#include <armadillo>

#include "mesh/Mesh.h"

class MeshBuilder{

public:

  MeshBuilder& set_spacing(const double& spacing) {
  
    if(spacing < 0) {
  
      throw std::runtime_error("Error: negative spacing.");
  
    }
  
    this->spacing = spacing;
    return *this;
  
  }
  
  MeshBuilder& set_control_points(const std::vector<arma::vec>& controlPoints) {
  
    this->controlPoints = controlPoints;
    return *this;
  
  }

  Mesh create() {
  
    compute_grid_size();
  
    Mesh mesh;

    mesh.set_vertices(sample_grid());
    mesh.set_faces(build_faces());
  
    return mesh;
  
  }

  private:

  void compute_grid_size() {
  
    this->minX = DBL_MAX;
    this->minY = DBL_MAX;
  
    this->maxX = -DBL_MAX;
    this->maxY = -DBL_MAX;
  
    for(const arma::vec& point: this->controlPoints) {
  
      this->minX = (point(0) < this->minX)? point(0): this->minX;
      this->minY = (point(1) < this->minY)? point(1): this->minY;
  
      this->maxX = (point(0) > this->maxX)? point(0): this->maxX;
      this->maxY = (point(1) > this->maxY)? point(1): this->maxY;
  
    }
  
  }

  std::vector<arma::vec> sample_grid() const {
  
    std::vector<arma::vec> samples;
  
    for(double x = this->minX; x <= this->maxX; x += this->spacing) {

      for(double y = this->minY; y <= this->maxY; y += this->spacing) {

        const arma::vec sample({x, y, 0});
        samples.push_back(sample);
  
      }

    }
  
    return samples;
  
  }

  std::vector< std::vector<unsigned int> > build_faces() const {
  
    const int sampleAmountX = ( this->maxX - this->minX ) / this->spacing + 1;
    const int sampleAmountY = ( this->maxY - this->minY ) / this->spacing + 1;

    // stride for moving to next row of grid
    const int stride = sampleAmountY;
  
    std::vector< std::vector<unsigned int> > faces;
  
    // construct left triangles
    // leave out right border
    for(int i = 0; i < sampleAmountX - 1; ++i) {
      // and bottom border
      for(int j = 0; j < sampleAmountY - 1; ++j) {
  
        const unsigned int topLeft = i * stride + j;
        const unsigned int bottomLeft = i * stride + j + 1;
        const unsigned int bottomRight = i * stride + j + 1 + stride;
  
        std::vector<unsigned int> face({topLeft, bottomLeft, bottomRight});
  
        faces.push_back(face);
  
      }
    }
  
    // construct right triangles
    // leave out left border
    for(int i = 1; i < sampleAmountX; ++i) {
      // and bottom border
      for(int j = 0; j < sampleAmountY - 1; ++j) {
  
        const unsigned int topRight = i * stride + j;
        const unsigned int topLeft = i * stride + j - stride;
        const unsigned int bottomRight = i * stride + j + 1;
  
        std::vector<unsigned int> face({topRight, topLeft, bottomRight});
  
        faces.push_back(face);
  
      }
    }
  
    return faces;
  
  }

  std::vector<arma::vec> controlPoints;

  double spacing = 1;

  double minX;
  double minY;

  double maxX;
  double maxY;

};

#endif
