#ifndef __IMAGE_ARITHMETIC_H__
#define __IMAGE_ARITHMETIC_H__

#include <stdexcept>

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageInfo.h"

class ImageArithmetic{

public:

  // constructor that initializes the ImageAccess object
  ImageArithmetic(ImageData& imageData) :
    imageData(imageData),
    imageAccess(imageData) {
  } 

  void plus(const ImageAccess& otherAccess, const ImageInfo& otherInfo ) {
  
    verify_compatibility(otherInfo);
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {
  
          this->imageAccess.at_grid(i, j, k) += otherAccess.at_grid(i, j, k);
  
        }
      }
    }
  
  }

  void minus(const ImageAccess& otherAccess, const ImageInfo& otherInfo) {
  
    verify_compatibility(otherInfo);
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {
  
          this->imageAccess.at_grid(i, j, k) -= otherAccess.at_grid(i, j, k);
  
        }
      }
    }
  
  }

private:

  void verify_compatibility(const ImageInfo& otherInfo) const  {
  
    if(
       otherInfo.get_nx() != this->imageData.nx ||
       otherInfo.get_ny() != this->imageData.ny ||
       otherInfo.get_nz() != this->imageData.nz ) {
  
      throw std::runtime_error("Can not perform image arithmetic: Images are incompatible.");
  
    }
  
  }

  const ImageData& imageData;
  ImageAccess imageAccess;

};

#endif
