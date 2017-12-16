#ifndef __IMAGE_MORPHOLOGY_H__
#define __IMAGE_MORPHOLOGY_H__

#include <cfloat>

#include "image/ImageAccess.h"
#include "image/ImageBoundary.h"
#include "image/ImageData.h"

class ImageMorphology{

public:

   // Sets the reference to the ImageData object and also initializes
   // ImageAccess and ImageArithmetic members
  ImageMorphology(
                  ImageData& imageData
                  ) :
    imageData(imageData),
    imageAccess(ImageAccess(imageData)) {
  
  }

  void dilation(
                const int& radius
                ) {
  
    // make copy of original data
    ImageData originalData = this->imageData;
    ImageAccess originalAccess(originalData);
  
    // adapt boundary and mirror
    ImageBoundary(originalData).change_boundary(radius, radius, radius);
    ImageMirror(originalData).all();
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {
  
          this->imageAccess.at_grid(i, j, k) = find(MAXIMUM, originalAccess, i, j, k, radius);
  
        }
      }
    }
  
  }

  void erosion(
               const int& radius
               ) {
  
    // make copy of original data
    ImageData originalData = this->imageData;
    ImageAccess originalAccess(originalData);
  
    // adapt boundary and mirror
    ImageBoundary(originalData).change_boundary(radius, radius, radius);
    ImageMirror(originalData).all();
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {
  
          this->imageAccess.at_grid(i, j, k) = find(MINIMUM, originalAccess, i, j, k, radius);
  
        }
      }
    }
  
  }

  void opening(
               const double& radius
               ) {
  
    erosion(radius);
    dilation(radius);
  
  }

  void closing(
               const double& radius
               ) {
  
    dilation(radius);
    erosion(radius);
  
  }

  void gradient(
                const double& radius
                ) {
  
    // copy image data
    ImageData copyData = this->imageData;
  
    // dilate original
    dilation(radius);
    ImageData dilatedData = this->imageData;
  
    // erode original
    this->imageData = copyData;
    erosion(radius);
  
    for(size_t i = 0; i < this->imageData.values.size(); ++i) {
  
      this->imageData.values[i] = dilatedData.values[i] - this->imageData.values[i];
  
    }
  
  }

  void white_top_hat(
                     const double& radius
                     ) {
  
    // copy image data
    ImageData originalData = this->imageData;
  
    // compute opening
    opening(radius);
  
    for(size_t i = 0; i < this->imageData.values.size(); ++i) {
  
      this->imageData.values[i] = originalData.values[i] - this->imageData.values[i];
  
    }
  
  }

  void black_top_hat(
                     const double& radius
                     ) {
    // copy image data
    ImageData originalData = this->imageData;
  
    // compute closing
    closing(radius);
  
    for(size_t i = 0; i < this->imageData.values.size(); ++i) {
  
      this->imageData.values[i] = this->imageData.values[i] - originalData.values[i];
  
    }
  
  }

private:

  enum Extremum{
    MINIMUM,
    MAXIMUM
  };

  // find extremum in given neighborhood centered at provided grid position
  double find(
              Extremum type,
              const ImageAccess& image,
              const int& x,
              const int& y,
              const int& z,
              const double& radius
              ) {
  
    double extremum = (type == MAXIMUM)? -DBL_MAX: DBL_MAX;
  
    for(int i = -radius; i <= radius; ++i) {
      for(int j = -radius; j <= radius; ++j) {
        for(int k = -radius; k <= radius; ++k) {
  
          const double& value = image.at_grid(x + i, y + j, z + k);
  
          switch(type){
  
          case MINIMUM:
            extremum = (value < extremum)? value: extremum;
            break;
  
          case MAXIMUM:
            extremum = (value > extremum)? value: extremum;
            break;
  
          }
  
        }
      }
    }
  
    return extremum;
  
  }

  ImageData& imageData;
  ImageAccess imageAccess;

};
#endif
