#ifndef __LANDMARK_SEGMENTATION_H__
#define __LANDMARK_SEGMENTATION_H__

#include <armadillo>

#include "image/ImageAccess.h"
#include "image/ImageData.h"
#include "image/ImageBoundary.h"
#include "image/segmentation/Threshold.h"

class LandmarkSegmentation{

private:

  ImageData& imageData;
  const std::vector<arma::vec> landmarks;

public:

  LandmarkSegmentation(
            ImageData& imageData,
            const std::vector<arma::vec>& landmarks
            ) :
    imageData(imageData),
    landmarks(landmarks) {

    ImageValues imageValues(this->imageData);
    imageValues.scale(0, 255);
    imageValues.round();

  }

  void apply() {

    const double bestThreshold = find_threshold();

    Threshold(this->imageData, bestThreshold).apply();

  }

private:

  double find_threshold() {

    ImageAccess access(this->imageData);

    double threshold = 255;

    for(const arma::vec& landmark: this->landmarks) {

      const double& currentColor = access.at_grid(landmark(0), landmark(1), landmark(2));
      threshold = (currentColor < threshold )? currentColor: threshold;

    }

    return threshold;

  }

};
#endif
