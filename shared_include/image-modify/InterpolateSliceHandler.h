#ifndef __INTERPOLATE_SLICE_HANDLER_H__
#define __INTERPOLATE_SLICE_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class InterpolateSliceHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];

    const std::string type = options["type"].asString();

    if      (type == "xy") { xy(image, options); }
    else if (type == "xz" ) { xz(image, options); }
    else if( type == "yz" ) { yz(image, options); }

  }

  void xy(Image& image, Json::Value& options) {

    const double position = options["position"].asDouble();

    image.interpolate_slice().at_xy(position);

  }

  void xz(Image& image, Json::Value& options) {

    const double position = options["position"].asDouble();

    image.interpolate_slice().at_xz(position);

  }

  void yz(Image& image, Json::Value& options) {

    const double position = options["position"].asDouble();

    image.interpolate_slice().at_yz(position);

  }

};
#endif
