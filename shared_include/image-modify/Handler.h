#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <json/json.h>
#include "image/Image.h"

class Handler{

public:

    virtual void handle(Image& image, Json::Value& modification) = 0;
    virtual ~Handler() {};

};

#endif
