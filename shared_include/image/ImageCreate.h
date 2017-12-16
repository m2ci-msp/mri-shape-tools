#ifndef __IMAGE_CREATE_H__
#define __IMAGE_CREATE_H__



#include "image/ImageData.h"

class ImageCreate{

public:

  // constructor that initializes the ImageData reference
  ImageCreate(const ImageData& imageData) : imageData(imageData) {
  }

  ImageCreate& with_dimension(const int& nx, 
                              const int& ny,
                              const int& nz) {
  
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;
  
    this->dimensionSet = true;
  
    return *this;
  
  }

  ImageCreate& with_boundary(const int& bx, 
                             const int& by,
                             const int& bz) {
  
    this->bx = bx;
    this->by = by;
    this->bz = bz;
  
    this->boundarySet = true;
  
    return *this;
  
  }

  ImageCreate& with_grid_spacing(const double& hx,
                                 const double& hy,
                                 const double& hz) {
  
    this->hx = hx;
    this->hy = hy;
    this->hz = hz;
  
    this->gridSpacingSet = true;
  
    return *this;
  
  }

  ImageCreate& with_origin(const double& originX,
                           const double& originY,
                           const double& originZ) {
  
    this->originX = originX;
    this->originY = originY;
    this->originZ = originZ;
  
    this->originSet = true;
  
    return *this;
  
  }

  // creates an empty image with the provided settings
  void empty_image() {
  
    if(this->dimensionSet == false) {
  
      throw std::runtime_error("empty_image: Can not create image. Dimensions were not set.");
  
    }
  
    // set meta data
    this->imageData.nx = this->nx;
    this->imageData.ny = this->ny;
    this->imageData.nz = this->nz;
  
    this->imageData.bx = this->bx;
    this->imageData.by = this->by;
    this->imageData.bz = this->bz;
  
    this->imageData.hx = this->hx;
    this->imageData.hy = this->hy;
    this->imageData.hz = this->hz;
  
    this->imageData.originX = originX;
    this->imageData.originY = originY;
    this->imageData.originZ = originZ;
  
    // allocate storage for image and initialize with 0
    this->imageData.values.resize(
                                  ( this->nx + 2 * this->bx ) *
                                  ( this->ny + 2 * this->by ) *
                                  ( this->nz + 2 * this->bz ),
                                  0
                                  );
  
    restore_defaults();
  
  }

  void image_from(const std::vector<double>& values) {
  
    if( this->dimensionSet == false || this->boundarySet == false ) {
  
      throw std::runtime_error("image_from: Can not create image. Dimensions or boundary sizes were not set.");
  
    }
  
    empty_image();
  
    if( this->imageData.values.size() != values.size() ) {
  
      throw std::runtime_error("image_from: Can not create image. Dimensions mismatch!");
  
    }
  
    this->imageData.values = values;
  
  }

private:

  void restore_defaults() {
  
      this->bx = 0;
      this->by = 0;
      this->bz = 0;
  
      this->hx = 0;
      this->hy = 0;
      this->hz = 0;
  
      this->originX = 0;
      this->originY = 0;
      this->originZ = 0;
  
      this->dimensionSet = false;
      this->boundarySet = false;
      this->gridSpacingSet = false;
      this->originSet = false;
  
    }

  ImageData& imageData;

  // dimension settings
  int nx;
  int ny;
  int nz;

  // boundary sizes
  int bx = 0;
  int by = 0;
  int bz = 0;

  // grid spacings
  double hx = 1;
  double hy = 1;
  double hz = 1;

  double originX = 0;
  double originY = 0;
  double originZ = 0;

};
#endif
