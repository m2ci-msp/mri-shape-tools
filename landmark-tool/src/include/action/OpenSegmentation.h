#ifndef __OPEN_SEGMENTATION_H__
#define __OPEN_SEGMENTATION_H__

#include "singleton/Data.h"
#include "core/SegmentationStackBuilder.h"

#include "image/Image.h"

class OpenSegmentation{

  public:

    OpenSegmentation(const std::string& path) {

      this->image.read().from(path);

    }

    /*----------------------------------------------------------------------*/

    virtual void execute() {

      SegmentationStackBuilder builder;

      Data::get_instance()->set_segmentation_stack(builder.build_from(image));

    }

    /*----------------------------------------------------------------------*/

 private:

    Image image;

};
#endif
