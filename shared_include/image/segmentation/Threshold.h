#ifndef __THRESHOLD_H__
#define __THRESHOLD_H__

#include "image/ImageData.h"

class Threshold{

public:

  Threshold(
            ImageData& imageData,
            const double& threshold
            ) :
    imageData(imageData),
    threshold(threshold) {
  }

  void apply() {

    for(double& value: this->imageData.values) {
      value = (value < this->threshold)? 0: 255;
    }

  }

private:

  ImageData& imageData;
  const double& threshold;

};
#endif
