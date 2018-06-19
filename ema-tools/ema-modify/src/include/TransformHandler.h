#ifndef __EMA_TRANSFORM_HANDLER_H__
#define __EMA_TRANSFORM_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "ema/Ema.h"

#include "matrix/Transformation.h"

class EmaTransformHandler : public Handler {

public:

  virtual void handle(Ema& ema, Json::Value& modification) {

    Json::Value& options = modification["options"];
    const std::string type = options["type"].asString();

    if      ( type == "translate") { translate(ema, options); }
    else if (type == "scale") { scale(ema, options); }
    else if ( type == "rotate") { rotate(ema, options) ;}

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

  void rotate(Ema& ema, Json::Value& options) {

    const std::string coilName = options["coil"].asString();

    // read axis
    const std::string axis = options["axis"].asString();

    // read angle
    double angle = options["angle"].asDouble();

    // convert angle to radians if needed
    if( options["angleUnit"].empty() == false) {

      if( options["angleUnit"].asString() == "degrees") {

        angle = angle / 180 * M_PI;

      }

    }

    // construct transformation depending on chosen rotation
    // axis
    Transformation transform;

    if( axis == "x" ) {

      transform = Transformation(angle, 0, 0);

    }
    else if ( axis == "y" ) {

      transform = Transformation(0, angle, 0);

    }
    else if( axis == "z" ) {

      transform = Transformation(0, 0, angle);

    }

    ema.coil(coilName).transform().apply_rigid_transformation(transform);

  }


};
#endif
