#ifndef __MESH_TRANSLATE_HANDLER_H__
#define __MESH_TRANSLATE_HANDLER_H__

#include "mesh/Mesh.h"

#include "mesh-modify/Handler.h"

namespace meshModify{

  class TranslateHandler : public Handler {

  public:

    virtual void handle(Mesh& mesh, const Json::Value& modification) {

      const Json::Value& options = modification["options"];

      const double x = options["x"].asDouble();
      const double y = options["y"].asDouble();
      const double z = options["z"].asDouble();

      for( arma::vec& vertex: mesh.get_vertices() ) {

        vertex(0) += x;
        vertex(1) += y;
        vertex(2) += z;

      }

    }

  };

}
#endif
