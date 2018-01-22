#ifndef __IMAGE_STATISTICS_H__
#define __IMAGE_STATISTICS_H__

#include <algorithm>

#include "image/ImageAccess.h"
#include "image/ImageData.h"

class ImageStatistics{

public:

  ImageStatistics(ImageData& imageData) :
    imageData(imageData),
    imageAccess(imageData) {
  }

  double percentile(const double& percentage) const {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    std::vector<double> values;

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {

          values.push_back(this->imageAccess.at_grid(i, j ,k));

        }
      }
    }

    std::sort(values.begin(), values.end());

    const int index = ( percentage / 100 ) * values.size() - 1;

    return values.at(index);

  }

private:

  ImageData& imageData;
  const ImageAccess imageAccess;

};

#endif
