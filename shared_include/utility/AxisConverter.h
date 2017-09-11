#ifndef __AXIS_CONVERTER_H__
#define __AXIS_CONVERTER_H__

#include <string>
#include <armadillo>

#include "mesh/Mesh.h"
#include "utility/AxisAccess.h"

class AxisConverter{

public:

  AxisConverter(
                const std::string& specification
                ) :
    mapping(specification),
    // use no scaling
    scaling({1, 1, 1}) {
  }

  AxisConverter(
                const std::string& specification,
                const arma::vec& scaling
                ) :
    mapping(specification),
    scaling(scaling) {
    }

  // map points using the chosen axis access
  // apply a scaling to the individual coordinates if desired
  arma::vec convert(const arma::vec& point) const {
  
    arma::vec result = arma::zeros(3);
  
    result(0) = this->scaling.at( this->mapping.x() ) * point( this->mapping.x() );
    result(1) = this->scaling.at( this->mapping.y() ) * point( this->mapping.y() );
    result(2) = this->scaling.at( this->mapping.z() ) * point( this->mapping.z() );
  
    return result;
  
  }

  Mesh convert(const Mesh& mesh) const {
  
    Mesh result(mesh);
  
    for( arma::vec& point: result.get_vertices() ){
      point = convert(point);
    }
  
    for( arma::vec& normal: result.get_vertex_normals() ){
      normal = convert(normal);
    }
  
    return result;
  
  }

private:

  const AxisAccess mapping;
  const arma::vec scaling;

};
#endif
