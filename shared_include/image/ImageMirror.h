#ifndef __IMAGE_MIRROR_H__
#define __IMAGE_MIRROR_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"

class ImageMirror{

public:

  ImageMirror(ImageData& imageData) :
    imageData(imageData),
    imageAccess(ImageAccess(imageData)) {
  }

  void mirror() {
  
      mirror_x();
      mirror_y();
      mirror_z();
  
  }

  void mirror_x() {
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;
  
    for(int z = -bz; z < nz + bz; ++z) {
      for(int y = -by; y < ny + by; ++y) {
        for(int x = 0; x < bx; ++x) {
  
          this->imageAccess.at_grid(- x - 1, y, z ) = this->imageAccess.at_grid(x         , y, z);
          this->imageAccess.at_grid(nx + x , y, z ) = this->imageAccess.at_grid(nx - 1 - x, y, z);
  
        } // end for x
      } // end for y
    } // end for z
  
  }

  void mirror_y() {
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;
  
    for(int x = -bx; x < nx + bx; ++x) {
      for(int z = -bz; z < nz + bz; ++z) {
        for(int y = 0; y < by; ++y) {
  
          this->imageAccess.at_grid(x, - y - 1, z ) = this->imageAccess.at_grid(x, y         , z);
          this->imageAccess.at_grid(x, ny + y , z ) = this->imageAccess.at_grid(x, ny - 1 - y, z);
  
        } // end for y
      } // end for z
    } // end for x
  
  }

  void mirror_z() {
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;
  
    for(int x = -bx; x < nx + bx; ++x) {
      for(int y = -by; y < ny + by; ++y) {
        for(int z = 0; z < bz; ++z) {
  
          this->imageAccess.at_grid(x, y, - z - 1) = this->imageAccess.at_grid(x, y, z         );
          this->imageAccess.at_grid(x, y, nz + z ) = this->imageAccess.at_grid(x, y, nz - 1 - z);
  
        } // end for z
      } // end for y
    } // end for x
  
  }

private:

  ImageData& imageData;

  ImageAccess imageAccess;

};
#endif
