#ifndef __MESH_HANDLER_H__
#define __MESH_HANDLER_H__

#include <json/json.h>
#include "mesh/Mesh.h"

namespace meshModify{

  class Handler{

  public:

    virtual void handle(Mesh&, const Json::Value&) = 0;
    virtual ~Handler() {}

  };

}
#endif
