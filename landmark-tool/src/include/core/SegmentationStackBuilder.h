#ifndef __SEGMENTATION_STACK_BUILDER_H__
#define __SEGMENTATION_STACK_BUILDER_H__

#include "core/ImageStackBuilder.h"

class SegmentationStackBuilder : public ImageStackBuilder {

 protected:

  virtual unsigned char compute_blue_component(const double& color) const {

    return ( color == 0 )? 255 : 0;

  }

  virtual unsigned char compute_green_component(const double&) const {

    return 0;

  }

  virtual unsigned char compute_red_component(const double& color) const {

    return ( color == 255 )? 255 : 0;

  }

};
#endif
