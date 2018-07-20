#ifndef __MAP_FROM_COORDINATE_SYSTEM_HANDLER_H__
#define __MAP_FROM_COORDINATE_SYSTEM_HANDLER_H__

#include "mesh/Mesh.h"
#include "ema/EmaCoordinateSystem.h"

#include "Handler.h"

class MapFromCoordinateSystemHandler : public Handler {

public:

  virtual void handle(Mesh& mesh, Json::Value& modification) {

    Json::Value& options = modification["options"];

    EmaCoordinateSystem system;

    system.build_from(options);

    // apply transformation to all vertices
    for( arma::vec& vertex: mesh.get_vertices() ) {

      vertex = system.map_back(vertex);

    }

  }

};

#endif
