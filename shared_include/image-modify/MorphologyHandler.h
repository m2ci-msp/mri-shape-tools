#ifndef __MORPHOLOGY_HANDLER_H__
#define __MORPHOLOGY_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class MorphologyHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];

    const std::string type = options["type"].asString();

    if      (type == "dilation") { dilation(image, options); }
    else if (type == "erosion" ) { erosion(image, options); }
    else if( type == "opening" ) { opening(image, options); }
    else if( type == "closing" ) { closing(image, options); }
    else if( type == "white top hat" ) { white_top_hat(image, options); }
    else if( type == "black top hat" ) { black_top_hat(image, options); }
    else if( type == "gradient" ) { gradient(image, options); }

  }

  void dilation(Image& image, Json::Value& options) {

    const int radius = options["radius"].asInt();
    image.morphology().dilation(radius);

  }

  void erosion(Image& image, Json::Value& options) {

    const int radius = options["radius"].asInt();
    image.morphology().erosion(radius);

  }

  void opening(Image& image, Json::Value& options) {

    const int radius = options["radius"].asInt();
    image.morphology().opening(radius);

  }

  void closing(Image& image, Json::Value& options) {

    const int radius = options["radius"].asInt();
    image.morphology().closing(radius);

  }

  void white_top_hat(Image& image, Json::Value& options) {

    const int radius = options["radius"].asInt();
    image.morphology().white_top_hat(radius);

  }

  void black_top_hat(Image& image, Json::Value& options) {

    const int radius = options["radius"].asInt();
    image.morphology().black_top_hat(radius);

  }

  void gradient(Image& image, Json::Value& options) {

    const int radius = options["radius"].asInt();
    image.morphology().gradient(radius);

  }

};
#endif
