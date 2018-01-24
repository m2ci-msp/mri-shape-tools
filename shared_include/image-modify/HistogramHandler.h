#ifndef __HISTOGRAM_HANDLER_H__
#define __HISTOGRAM_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class HistogramHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];
    const std::string type = options["type"].asString();

    if      (type == "equalize") { equalize(image); }

  }

  void equalize(Image& image) {

    image.histogram().equalize();

  }

};
#endif
