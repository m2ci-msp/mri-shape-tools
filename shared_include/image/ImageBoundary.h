#ifndef __IMAGE_BOUNDARY_H__
#define __IMAGE_BOUNDARY_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"

class ImageBoundary{

public:

  // constructor that initializes the ImageData reference
  // and creates the ImageAccess object
  ImageBoundary(ImageData& imageData) :
    imageData(imageData),
    imageAccess(imageData) {
  }

  void change(
              const int& bx,
              const int& by,
              const int& bz
              ) {
  
    // check if update is needed
    if( bx == this->imageData.bx && by == this->imageData.by && bz == this->imageData.bz ) {
  
      // no update necessary
      return;
  
    }
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    // create copy of image data
    ImageData originalData = this->imageData;
  
    // create access to original data
    ImageAccess originalAccess(originalData);
  
    // update boundary sizes
    this->imageData.bx = bx;
    this->imageData.by = by;
    this->imageData.bz = bz;
  
    // resize data storage
    this->imageData.values.resize( ( 2 * bx + nx ) * ( 2 * by + ny ) * ( 2 * bz + nz ) );
  
    // copy original data
    for(int x = 0; x < nx; ++x) {
      for(int y = 0; y < ny; ++y) {
        for(int z = 0; z < nz; ++z) {
  
          this->imageAccess.at_grid(x, y, z) = originalAccess.at_grid(x, y, z);
  
        } // end for z
      } // end for y
    } // end for x
  
  }

private:

  ImageData& imageData;
  ImageAccess imageAccess;

};

#endif
