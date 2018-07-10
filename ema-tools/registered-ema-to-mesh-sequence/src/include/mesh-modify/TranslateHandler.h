#ifndef __TRANSLATE_HANDLER_H__
#define __TRANSLATE_HANDLER_H__

#include "mesh/Mesh.h"

#include "Handler.h"

class TranslateHandler : public Handler {

public:

  virtual void handle(Mesh& mesh, Json::Value& modification) {

    Json::Value& options = modification["options"];

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

#endif
