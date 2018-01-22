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
    const int minY = options["minX"].asInt();
    const int minZ = options["minX"].asInt();

    const int maxX = options["minX"].asInt();
    const int maxY = options["minX"].asInt();
    const int maxZ = options["minX"].asInt();

    image.crop().to(minX, minY, minZ, maxX, maxY, maxZ);

  }

};
#endif
