#ifndef __EMA_TRANSFORM_ALL_HANDLER_H__
#define __EMA_TRANSFORM_ALL_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "ema/Ema.h"

class EmaTransformAllHandler : public Handler {

public:

  virtual void handle(Ema& ema, Json::Value& modification) {

    Json::Value& options = modification["options"];
    const std::string type = options["type"].asString();

    if      ( type == "translate") { translate(ema, options); }
    else if (type == "scale") { scale(ema, options); }

  }

  void translate(Ema& ema, Json::Value& options) {

    const double x = options["x"].asDouble();
    const double y = options["y"].asDouble();
    const double z = options["z"].asDouble();

    ema.transform_all_coils().translate({x, y, z});

  }

  void scale(Ema& ema, Json::Value& options) {

    const double factor = options["factor"].asDouble();

    ema.transform_all_coils().scale(factor);

  }

};
#endif
