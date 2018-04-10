#ifndef __OPEN_SCAN_H__
#define __OPEN_SCAN_H__

#include "action/Action.h"
#include "singleton/Data.h"
#include "core/ImageStackBuilder.h"

#include "image/Image.h"

class OpenScan : public Action {

 public:

  OpenScan(const std::string& path) {

    this->image.read().from(path);

  }

  /*----------------------------------------------------------------------*/

  virtual void execute() {

    ImageStackBuilder builder;

    Data::get_instance()->set_scan(this->image);
    Data::get_instance()->set_image_stack(builder.build_from(this->image));

  }

  /*----------------------------------------------------------------------*/

 private:

  Image image;

};
#endif
