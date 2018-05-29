#ifndef __EMA_TRANSFORM_HANDLER_H__
#define __EMA_TRANSFORM_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "ema/Ema.h"

class EmaTransformHandler : public Handler {

public:

  virtual void handle(Ema& ema, Json::Value& modification) {

    Json::Value& options = modification["options"];
    const std::string type = options["type"].asString();

    if      ( type == "translate") { translate(ema, options); }
    else if (type == "scale") { scale(ema, options); }

  }

  void translate(Ema& ema, Json::Value& options) {

    const std::string coilName = options["coil"].asString();
    const double x = options["x"].asDouble();
    const double y = options["y"].asDouble();
    const double z = options["z"].asDouble();

    ema.coil(coilName).transform().translate({x, y, z});

  }

  void scale(Ema& ema, Json::Value& options) {

    const std::string coilName = options["coil"].asString();
    const double factor = options["factor"].asDouble();

    ema.coil(coilName).transform().scale(factor);

  }

};
#endif
