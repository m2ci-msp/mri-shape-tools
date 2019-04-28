#ifndef __EMA_HANDLER_H__
#define __EMA_HANDLER_H__

#include <json/json.h>
#include "ema/Ema.h"

namespace emaModify{

  class Handler{

  public:

    virtual void handle(Ema&, Json::Value&) = 0;
    virtual ~Handler() {}

  };

}
#endif
