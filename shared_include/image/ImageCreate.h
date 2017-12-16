#ifndef __IMAGE_CREATE_H__
#define __IMAGE_CREATE_H__

#include <stdexcept>

#include "image/ImageData.h"

class ImageCreate{

public:

    ImageCreate(ImageData& imageData) : imageData(imageData) {
    }

    void empty_image(
        // dimensions
        const int nx,
        const int ny,
        const int nz,
        // boundary sizes
        const int bx = 0,
        const int by = 0,
        const int bz = 0,
        // grid spacings
        const double hx = 1,
        const double hy = 1,
        const double hz = 1,
        // origin coordinates
        const double orgX = 0,
        const double orgY = 0,
        const double orgZ = 0
        ) {
    
        // set meta data
        this->imageData.nx = nx;
        this->imageData.ny = ny;
        this->imageData.nz = nz;
    
        this->imageData.bx = bx;
        this->imageData.by = by;
        this->imageData.bz = bz;
    
        this->imageData.hx = hx;
        this->imageData.hy = hy;
        this->imageData.hz = hz;
    
        this->imageData.orgX = orgX;
        this->imageData.orgY = orgY;
        this->imageData.orgZ = orgZ;
    
        // allocate storage for image and initialize with 0
        this->imageData.values.resize(
          ( nx + 2 * bx ) *
          ( ny + 2 * by ) *
          ( nz + 2 * bz ),
          0
        );
    
    }

    void image_with_existing_data(
        // existing data
        const std::vector<double>& values,
        // dimensions
        const int nx,
        const int ny,
        const int nz,
        // boundary sizes
        const int bx,
        const int by,
        const int bz,
        // grid spacings
        const double hx,
        const double hy,
        const double hz,
        // origin coordinates
        const double orgX = 0,
        const double orgY = 0,
        const double orgZ = 0
    ) {
    
      // first create an empty image
      empty_image(nx, ny, nz, bx, by, bz, hx, hy, hz, orgX, orgY, orgZ);
    
      // verify that the size of the provided image data is correct
      if( this->imageData.values.size() != values.size() ) {
        throw std::runtime_error("Can't initialize image. Dimensions are incorrect.");
      }
    
      // initialize image data to provided values
      this->imageData.values = values;
    
    }

private:

   ImageData& imageData;

};
#endif
