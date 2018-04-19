#ifndef __IMAGE_EXTREMA_H__
#define __IMAGE_EXTREMA_H__

#include <cfloat>

#include "image/ImageAccess.h"
#include "image/ImageData.h"

class ImageExtrema{

public:

  ImageExtrema(ImageData& imageData) :
    imageData(imageData),
    imageAccess(imageData) {
  }

  void find(double& min, double& max) const {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    min =   DBL_MAX;
    max = - DBL_MAX;

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {

          const double& value = this->imageAccess.at_grid(i, j ,k);

          min = ( value < min )? value: min;
          max = ( value > max )? value: max;

        }
      }
    }

  }

  double get_min() const {

    double min, max;

    find(min, max);

    return min;

  }

  double get_max() const {

    double min, max;

    find(min, max);

    return max;

  }

private:

  ImageData& imageData;
  const ImageAccess imageAccess;

};

#endif
