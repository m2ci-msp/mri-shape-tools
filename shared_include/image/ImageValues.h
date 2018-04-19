#ifndef __IMAGE_VALUES_H__
#define __IMAGE_VALUES_H__

#include <vector>
#include <algorithm>

#include "image/ImageData.h"
#include "image/ImageAccess.h"

class ImageValues{

public:

  ImageValues(ImageData& imageData) :
    imageData(imageData),
    imageAccess(imageData) {

  }

  /* linearly scales values to the interval [minNew, maxNew] */
  void scale(const double& minNew, const double& maxNew) {
  
    if( minNew > maxNew ) {
      throw std::runtime_error("Cannot scale values: min > max");
    }
  
    double min, max;
    ImageExtrema(this->imageData).find(min, max);
  
    const double dist = max - min;
    const double distNew = maxNew - minNew;
  
    for(double& value: this->imageData.values) {
      // shift to zero
      value -= min;
  
      // rescale
      value *= distNew / dist;
  
      // shift to minNew
      value += minNew;
    }
  
  }

  // adds a value to all gray values in a specified sub image
  void add(const double& value,
           const int& minX,
           const int& minY,
           const int& minZ,
           const int& maxX,
           const int& maxY,
           const int& maxZ) {

    for(int i = minX; i < maxX; ++i) {
      for(int j = minY; j < maxY; ++j) {
        for(int k = minZ; k < maxZ; ++k) {

          this->imageAccess.at_grid(i, j, k) += value;

        }
      }
    }

  }

  void normalize() {
  
    scale(0, 1);
  
  }

  void round() {
    for(double& value: this->imageData.values) {
  
      value = (int) value;
  
    }
  
  }

  /* Discards colors at the lower and upper end of the color range.
     Combined with the scale_values method, it reproduces the method
     described on
  
     http://www.imagemagick.org/Usage/color_mods/#normalize
  
     to enhance the contrast.
  */
  void discard(
    // percentage of darkest colors to discard
    const double& lower,
    // percentage of brightest colors to discard
    const double& upper
    ) {
  
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    // order values
    std::vector<double> sortedVector;
  
    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {
          sortedVector.push_back( this->imageAccess.at_grid(i, j, k) );
        }
      }
    }
  
    std::sort(sortedVector.begin(), sortedVector.end());
  
    // get indices
    int lowerIndex = sortedVector.size() / 100. * lower;
    int upperIndex = sortedVector.size() / 100. * (100 - upper);
  
    // get new minimum and maximum
    const double newMin = sortedVector.at(lowerIndex);
    const double newMax = sortedVector.at(upperIndex);
  
    // discard colors
    for(double& value: this->imageData.values) {
      value = (value < newMin )? newMin : value;
      value = (value > newMax )? newMax : value;
    }
  
  }

private:

  ImageData& imageData;

  ImageAccess imageAccess;

};

#endif
