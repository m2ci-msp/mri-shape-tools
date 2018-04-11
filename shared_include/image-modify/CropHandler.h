#ifndef __CROP_HANDLER_H__
#define __CROP_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class CropHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];
    crop(image, options);

  }

  void crop(Image& image, Json::Value& options) {

    const int minX = options["minX"].asInt();
    const int minY = options["minY"].asInt();
    const int minZ = options["minZ"].asInt();

    const int maxX = options["maxX"].asInt();
    const int maxY = options["maxY"].asInt();
    const int maxZ = options["maxZ"].asInt();

    image.crop().to(minX, minY, minZ, maxX, maxY, maxZ);

  }

};
#endif
