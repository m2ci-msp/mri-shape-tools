#ifndef __CASCADED_OTSU_H__
#define __CASCADED_OTSU_H__

#include "image/segmentation/Otsu.h"
#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageBoundary.h"

class CascadedOtsu{

public:

  CascadedOtsu(
               ImageData& imageData,
               const int& iterations) :
    imageData(imageData), imageCopy(imageData), iterations(iterations) {

    // remove boundary in copy
    ImageBoundary(imageCopy).change_boundary(0, 0, 0);

  }

  void apply() {

    // setup
    this->segmentation = this->imageCopy;
    Otsu(this->segmentation).apply();

    double mean = compute_mean();

    overwrite_foreground(mean);

    // perform iterations
    for(int i = 0; i < this->iterations; ++i) {

      this->segmentation = this->imageCopy;
      Otsu(this->segmentation).apply();

      mean = compute_mean();

      overwrite_foreground(mean);

    }

    // write result
    create_result();

  }

private:

  double compute_mean() const {

    int index = 0;
    double sum = 0;

    for(const double& value: this->segmentation.values) {

      if(value == 255) {
        sum += this->imageCopy.values.at(index);
      }

      ++index;

    }

    return sum / this->imageCopy.values.size();

  }

  void overwrite_foreground(const double& mean) {

    int index = 0;

    for(const double& value: this->segmentation.values) {

      if(value == 255) {
        this->imageCopy.values.at(index) = mean;
      }

      ++index;

    }

  }

  void create_result() {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    ImageAccess imageAccess(this->imageData);
    ImageAccess segmentationAccess(this->segmentation);

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {

          imageAccess.at_grid(i, j, k) = segmentationAccess.at_grid(i, j, k);

        }
      }
    }

  }

  ImageData& imageData;

  ImageData imageCopy;
  ImageData segmentation;

  const int iterations;

};

#endif
