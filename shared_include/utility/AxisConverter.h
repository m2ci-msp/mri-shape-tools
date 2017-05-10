#ifndef __AXIS_CONVERTER_H__
#define __AXIS_CONVERTER_H__

#include <string>

#include <armadillo>

#include "mesh/Mesh.h"

class AxisConverter{

public:

  /*--------------------------------------------------------------------------*/

  AxisConverter(
    const std::string& mapping,
    const arma::vec& factors
    ) : factors(factors) {

    this->axisMapping = determine_axis_access(mapping);

  }

  /*--------------------------------------------------------------------------*/

  arma::vec convert(const arma::vec& point) const {

    arma::vec result = arma::zeros(3);

    for( int i = 0; i < 3; ++i ) {

      result(i) = this->factors.at( this->axisMapping(i) ) * point( this->axisMapping(i) );

    }

    return result;

  }

  /*--------------------------------------------------------------------------*/

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

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  arma::vec determine_axis_access(const std::string& specification) const {

    arma::vec result = arma::zeros(3);

    if( specification == "XYZ" ) {

      result(0) = 0;
      result(1) = 1;
      result(2) = 2;

    }
    else if ( specification == "ZXY" ) {

      result(0) = 2;
      result(1) = 0;
      result(2) = 1;

    }
    else if ( specification == "YZX" ) {

      result(0) = 1;
      result(1) = 2;
      result(2) = 0;

    } else if ( specification == "XZY" ) {

      result(0) = 0;
      result(1) = 2;
      result(2) = 1;

    } else {
      throw std::runtime_error("Unknown axis specification.");
    }

    return result;

  }

  /*--------------------------------------------------------------------------*/

  arma::vec axisMapping;
  arma::vec factors;

  /*--------------------------------------------------------------------------*/

};

#endif
