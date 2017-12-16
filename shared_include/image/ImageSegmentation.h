#ifndef __IMAGE_SEGMENTATION_H__
#define __IMAGE_SEGMENTATION_H__

#include "image/ImageData.h"
#include "image/segmentation/Threshold.h"
#include "image/segmentation/Otsu.h"
#include "image/segmentation/CascadedOtsu.h"

class ImageSegmentation{

public:

  ImageSegmentation(ImageData& imageData) : imageData(imageData) {
  }

  void threshold(const double& t) {
      Threshold(this->imageData, t).apply();
  }

  void otsu() {
      Otsu(this->imageData).apply();
  }

  void cascaded_otsu(const double& iterations) {
      CascadedOtsu(this->imageData, iterations).apply();
  }

  private:

  ImageData& imageData;

};

#endif
