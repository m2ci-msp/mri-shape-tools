#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <json/json.h>
#include "mesh/Mesh.h"

class Handler{

public:

  virtual void handle(Mesh&, Json::Value&) = 0;
  virtual ~Handler() {}

};

#endif
