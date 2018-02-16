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
    else if( type == "add" ) { add(image, options); }

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

    image.values().discard(lower, upper);

  }

  void add(Image& image, Json::Value& options) {

    Json::Value nullValue;

    const int& nx = image.info().get_nx();
    const int& ny = image.info().get_ny();
    const int& nz = image.info().get_nz();

    const int minX = ( options["minX"] == nullValue)? 0: options["minX"].asInt();
    const int minY = ( options["minY"] == nullValue)? 0: options["minY"].asInt();
    const int minZ = ( options["minZ"] == nullValue)? 0: options["minZ"].asInt();

    const int maxX = ( options["maxX"] == nullValue)? nx: options["maxX"].asInt();
    const int maxY = ( options["maxY"] == nullValue)? ny: options["maxY"].asInt();
    const int maxZ = ( options["maxZ"] == nullValue)? nz: options["maxZ"].asInt();

    const double value = options["value"].asDouble();

    image.values().add(value, minX, minY, minZ, maxX, maxY, maxZ);

  }

};
#endif
