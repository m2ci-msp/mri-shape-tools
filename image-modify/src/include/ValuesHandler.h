#ifndef __VALUES_HANDLER_H__
#define __VALUES_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class ValuesHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];

    const std::string type = options["type"].asString();

    if      (type == "scale") { scale(image, options); }
    else if (type == "normalize" ) { normalize(image); }
    else if( type == "discard" ) { discard(image, options); }

  }

  void scale(Image& image, Json::Value& options) {

    const double minNew = options["minNew"].asDouble();
    const double maxNew = options["maxNew"].asDouble();

    image.values().scale(minNew, maxNew);

  }

  void normalize(Image& image) {

    image.values().normalize();

  }

  void discard(Image& image, Json::Value& options) {

    const int lower = options["lower"].asInt();
    const int upper = options["upper"].asInt();

    image.values().discard_values(lower, upper);

  }

};
#endif
