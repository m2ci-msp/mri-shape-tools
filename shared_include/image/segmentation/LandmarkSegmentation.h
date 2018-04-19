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

    for(int i = 254; i > 0; --i) {

      ImageData imageCopy = this->imageData;

      // remove boundary in copy
      ImageBoundary(imageCopy).change(0, 0, 0);

      Threshold(imageCopy, i).apply();

      if(all_landmarks_inside(imageCopy)) {

        return i;

      }

    }

    return 0;

  }

  bool all_landmarks_inside(ImageData& segmentation) const {

    ImageAccess access(segmentation);

    for(const arma::vec& landmark: this->landmarks) {

      if( access.at_grid(landmark(0), landmark(1), landmark(2)) != 255) {

        return false;

      }

    }

    return true;

  }


};
#endif
