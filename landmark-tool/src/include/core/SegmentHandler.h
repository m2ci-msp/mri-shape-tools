#ifndef __SEGMENT_HANDLER_H__
#define __SEGMENT_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class SegmentHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];

    const std::string type = options["type"].asString();

    if      (type == "threshold") { threshold(image, options); }
    else if (type == "otsu" ) { otsu(image); }
    else if (type == "cascaded otsu" ) { cascaded_otsu(image, options); }

  }

  void threshold(Image& image, Json::Value& options) {

    const double threshold = options["threshold"].asDouble();

    image.segment().threshold(threshold);

  }

  void otsu(Image& image) {

    image.segment().otsu();

  }

  void cascaded_otsu(Image& image, Json::Value& options) {

    const int iterations = options["iterations"].asInt();

    image.segment().cascaded_otsu(iterations);

  }
};
#endif