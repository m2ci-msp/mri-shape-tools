#ifndef __IMAGE_INFO_H__
#define __IMAGE_INFO_H__

#include "image/ImageData.h"

class ImageInfo{

public:

  // constructor that initializes the ImageData reference
  ImageInfo(const ImageData& imageData) : imageData(imageData) {
  }

  // getters for resolution
  const int& get_nx() const { return this->imageData.nx; }
  const int& get_ny() const { return this->imageData.ny; }
  const int& get_nz() const { return this->imageData.nz; }

  // getters for boundary sizes
  const int& get_bx() const { return this->imageData.bx; }
  const int& get_by() const { return this->imageData.by; }
  const int& get_bz() const { return this->imageData.bz; }

  // getters for grid spacings
  const double& get_hx() const { return this->imageData.hx; }
  const double& get_hy() const { return this->imageData.hy; }
  const double& get_hz() const { return this->imageData.hz; }

  // getters for image origin
  const double& get_origin_x() const { return this->imageData.originX; }
  const double& get_origin_y() const { return this->imageData.originY; }
  const double& get_origin_z() const { return this->imageData.originZ; }

private:

  const ImageData& imageData;

  };
#endif
