#ifndef __MESH_MAP_TO_COORDINATE_SYSTEM_HANDLER_H__
#define __MESH_MAP_TO_COORDINATE_SYSTEM_HANDLER_H__

#include "mesh/Mesh.h"
#include "ema/EmaCoordinateSystem.h"

#include "mesh-modify/Handler.h"

namespace meshModify{

  class MapToCoordinateSystemHandler : public Handler {

  public:

    virtual void handle(Mesh& mesh, const Json::Value& modification) {

      const Json::Value& options = modification["options"];

      EmaCoordinateSystem system;

      system.build_from(options);

      // apply transformation to all vertices
      for( arma::vec& vertex: mesh.get_vertices() ) {

        vertex = system.map(vertex);

      }

    }

  };

}
#endif
