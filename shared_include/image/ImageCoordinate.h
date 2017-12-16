#ifndef __IMAGE_COORDINATE_H__
#define __IMAGE_COORDINATE_H__

#include "image/ImageData.h"

class ImageCoordinate{

public:

  // constructor that initializes the ImageData reference
  ImageCoordinate(const ImageData& imageData) : imageData(imageData) {
  }

   void from_grid(
                 const int& gridX,
                 const int& gridY,
                 const int& gridZ,
                 double& x,
                 double& y,
                 double& z
                 ) const {
  
    x = gridX * this->imageData.hx + this->imageData.orgX;
    y = gridY * this->imageData.hy + this->imageData.orgY;
    z = gridZ * this->imageData.hz + this->imageData.orgZ;
  
  }

  // int result
  // to be used with ImageAccess
  void to_grid(
               const double& x,
               const double& y,
               const double& z,
               int& gridX,
               int& gridY,
               int& gridZ
               ) {
  
    gridX = (x - this->imageData.orgX ) / this->imageData.hx;
    gridY = (y - this->imageData.orgY ) / this->imageData.hy;
    gridZ = (z - this->imageData.orgZ ) / this->imageData.hz;
  
  }
  
  // double result
  // computed grid positions may be used for interpolation later on
  void to_grid(
               const double& x,
               const double& y,
               const double& z,
               double& gridX,
               double& gridY,
               double& gridZ
               ) {
  
    gridX = (x - this->imageData.orgX ) / this->imageData.hx;
    gridY = (y - this->imageData.orgY ) / this->imageData.hy;
    gridZ = (z - this->imageData.orgZ ) / this->imageData.hz;
  
  }

private:

  const ImageData& imageData;

};
#endif
