#ifndef __MESH_SCALE_HANDLER_H__
#define __MESH_SCALE_HANDLER_H__

#include "mesh/Mesh.h"

#include "mesh-modify/Handler.h"

namespace meshModify{

  class ScaleHandler : public Handler {

  public:

    virtual void handle(Mesh& mesh, const Json::Value& modification) {

      const Json::Value& options = modification["options"];

      const double factor = options["factor"].asDouble();

      for( arma::vec& vertex: mesh.get_vertices() ) {

        vertex(0) *= factor;
        vertex(1) *= factor;
        vertex(2) *= factor;

      }

    }

  };

}
#endif
