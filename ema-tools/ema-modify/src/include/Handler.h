#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <json/json.h>
#include "ema/Ema.h"

class Handler{

public:

  virtual void handle(Ema&, Json::Value&) = 0;
  virtual ~Handler() {}

};

#endif
