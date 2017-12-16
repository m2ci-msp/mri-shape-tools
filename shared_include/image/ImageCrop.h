#ifndef __IMAGE_CROP_H__
#define __IMAGE_CROP_H__

#include <stdexcept>

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageCreate.h"

class ImageCrop{

public:

  ImageCrop(ImageData& imageData) :
    imageData(imageData),
    imageAccess(ImageAccess(imageData))
  {
  }

  
  void to(
          const int& minX,
          const int& minY,
          const int& minZ,
          const int& maxX,
          const int& maxY,
          const int& maxZ
          ) {
  
    verify(minX, minY, minZ, maxX, maxY, maxZ);
  
    // compute new dimensions
    // we also include the boundary points of the bounding box
    const int nxNew = maxX - minX + 1;
    const int nyNew = maxY - minY + 1;
    const int nzNew = maxZ - minZ + 1;
  
    // compute new origin
    const double originXNew = this->imageData.originX + minX * this->imageData.hx;
    const double originYNew = this->imageData.originY + minY * this->imageData.hy;
    const double originZNew = this->imageData.originZ + minZ * this->imageData.hz;
  
    ImageData croppedData;
  
    ImageCreate(croppedData).with_dimension(nxNew, nyNew, nzNew)
                            .with_boundary(0, 0, 0)
                            .with_grid_spacing(this->imageData.hx, this->imageData.hy, this->imageData.hz)
                            .with_origin(originXNew, originYNew, originZNew)
                            .empty_image();

    ImageAccess cropped(croppedData);
  
    for(int i = minX; i <= maxX; ++i) {
      for(int j = minY; j <= maxY; ++j) {
        for(int k = minZ; k <= maxZ; ++k) {
  
          cropped.at_grid(i, j, k) = this->imageAccess.at_grid(i, j, k);
  
        }
      }
    }
  
    // replace original image data
    this->imageData = croppedData;
  
  }

private:

  void verify(
              const int& minX,
              const int& minY,
              const int& minZ,
              const int& maxX,
              const int& maxY,
              const int& maxZ
              ) {
  
    if( minX < 0 ) { throw std::runtime_error("Cannot crop image: minX < 0"); }
    if( minY < 0 ) { throw std::runtime_error("Cannot crop image: minY < 0"); }
    if( minZ < 0 ) { throw std::runtime_error("Cannot crop image: minZ < 0"); }
  
    if( maxX > this->imageData.nx ) { throw std::runtime_error("Cannot crop image: maxX > nx"); }
    if( maxY > this->imageData.ny ) { throw std::runtime_error("Cannot crop image: maxY > ny"); }
    if( maxZ > this->imageData.nz ) { throw std::runtime_error("Cannot crop image: maxZ > nz"); }
  
    if( minX > maxX ) { throw std::runtime_error("Cannot crop image: minX > maxX"); }
    if( minY > maxY ) { throw std::runtime_error("Cannot crop image: minY > maxY"); }
    if( minZ > maxZ ) { throw std::runtime_error("Cannot crop image: minZ > maxZ"); }
  
  }

  ImageData& imageData;

  ImageAccess imageAccess;

};

#endif
