#ifndef __MAP_TO_COORDINATE_SYSTEM_HANDLER_H__
#define __MAP_TO_COORDINATE_SYSTEM_HANDLER_H__

#include "mesh/Mesh.h"
#include "ema/EmaCoordinateSystem.h"

#include "Handler.h"

class MapToCoordinateSystemHandler : public Handler {

public:

  virtual void handle(Mesh& mesh, Json::Value& modification) {

    Json::Value& options = modification["options"];

    EmaCoordinateSystem system;

    system.build_from(options);

    // apply transformation to all vertices
    for( arma::vec& vertex: mesh.get_vertices() ) {

      vertex = system.map(vertex);

    }

  }

};
#endif
