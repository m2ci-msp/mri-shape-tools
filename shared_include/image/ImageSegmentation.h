#ifndef __IMAGE_SEGMENTATION_H__
#define __IMAGE_SEGMENTATION_H__

#include "image/ImageData.h"
#include "image/segmentation/Threshold.h"
#include "image/segmentation/Otsu.h"
#include "image/segmentation/IteratedOtsu.h"
#include "image/segmentation/LandmarkSegmentation.h"
#include "image/segmentation/ChanVese.h"
#include "image/segmentation/chanvese/ChanVeseSettings.h"

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

  void iterated_otsu(const int& iterations) {
    IteratedOtsu(this->imageData, iterations).apply();
  }

  void with_landmarks(const std::vector<arma::vec>& landmarks) {

    LandmarkSegmentation(this->imageData, landmarks).apply();

  }

  void chan_vese(
                 ImageData& levelSetData,
                 const ChanVeseSettings& settings
                 ) {

    ChanVese(this->imageData, levelSetData, settings).apply();

  }

private:

  ImageData& imageData;

};

#endif
