#ifndef __IMAGE_HISTOGRAM_H__
#define __IMAGE_HISTOGRAM_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageValues.h"

class ImageHistogram{

public:

  ImageHistogram(ImageData& imageData) :
    imageData(imageData),
    imageAccess(imageData),
    imageValues(imageData) {

  }

  void equalize() {

    // scale image values
    this->imageValues.scale(0, 255);
    this->imageValues.round();

    build_histogram();

    equalize_histogram();

  }

private:

  void build_histogram() {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    this->histogram.resize(255, 0);

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for( int k = 0; k < nz; ++k) {

          const double value = this->imageAccess.at_grid(i, j, k);
          this->histogram[(int) value] += 1;

        }
      }
    }

    const int size = nx * ny * nz;

    for(double& value: this->histogram) {

      value /= size;

    }

  }

  void equalize_histogram() {

    for( double& value: this->imageData.values ) {

      double sum = 0;
      int index = (int) value;

      for(int i = 0; i < index; ++i) {
        sum += this->histogram[i];
      }

      value = (int) 255 * sum;

    }

  }

  std::vector<double> histogram;

  ImageData& imageData;

  ImageAccess imageAccess;

  ImageValues imageValues;

};

#endif
