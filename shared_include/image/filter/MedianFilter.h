#ifndef __MEDIAN_FILTER_H__
#define __MEDIAN_FILTER_H__

#include <algorithm>

#include "image/ImageAccess.h"
#include "image/ImageBoundary.h"
#include "image/ImageData.h"
#include "image/ImageMirror.h"

class MedianFilter{

public:

  MedianFilter(
                 ImageData& imageData,
                 const double& radius
    ) :
    imageData(imageData),
    imageAccess(ImageAccess(imageData)),
    imageBoundary(ImageBoundary(imageData)),
    imageMirror(ImageMirror(imageData)),
    radius(radius) {

    adjust_image_boundaries();

  }

  void apply() {

    if( this->radius == 0) {
      return;
    }

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    this->imageMirror.all();

    // create image copy
    ImageData copyData = this->imageData;
    ImageAccess copyAccess(copyData);

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {

          this->imageAccess.at_grid(i, j, k) = find_median(copyAccess, i, j, k);

        }
      }
    }

  }

private:

  double find_median(
                     const ImageAccess& image,
                     const int& x,
                     const int& y,
                     const int& z
                     ) const {

    std::vector<double> values;

    for(int i = -this->radius; i < this->radius; ++i) {
      for(int j = -this->radius; j < this->radius; ++j) {
        for(int k = -this->radius; k < this->radius; ++k) {

          values.push_back(image.at_grid(x + i, y + j, z + k));

        }
      }
    }

    std::sort(values.begin(), values.end());

    // compute index of center value ->
    // size is odd and indexes start at 0
    const int center = ( values.size() - 1 ) / 2;

    return values[center];

  }

  void adjust_image_boundaries() {

    if( this->imageData.bx < this->radius ||
        this->imageData.by < this->radius ||
        this->imageData.bz < this->radius
      ) {

      this->imageBoundary.change_boundary(radius, radius, radius);

    }

  }

  ImageData& imageData;

  ImageAccess imageAccess;

  ImageBoundary imageBoundary;

  ImageMirror imageMirror;

  const double radius;

};

#endif
