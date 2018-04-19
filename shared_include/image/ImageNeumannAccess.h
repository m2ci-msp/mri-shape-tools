#ifndef __IMAGE_NEUMANN_ACCESS_H__
#define __IMAGE_NEUMANN_ACCESS_H__

#include "image/ImageAccess.h"

class ImageNeumannAccess: public ImageAccess{

public:

  // constructor that initializes the ImageData reference
  ImageNeumannAccess(ImageData& imageData) : ImageAccess(imageData) {
  }

  virtual double& at_grid(
                  const int& x,
                  const int& y,
                  const int& z
                  ) {

    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;

    const int accessX = compute_index_neumann( x, this->imageData.nx );
    const int accessY = compute_index_neumann( y, this->imageData.ny );
    const int accessZ = compute_index_neumann( z, this->imageData.nz );

    return this->imageData.values.at( compute_index(bx + accessX, by + accessY, bz + accessZ) );

  }

  // const version
  virtual const double& at_grid(
                        const int& x,
                        const int& y,
                        const int& z
                        ) const {

    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;

    const int accessX = compute_index_neumann( x, this->imageData.nx );
    const int accessY = compute_index_neumann( y, this->imageData.ny );
    const int accessZ = compute_index_neumann( z, this->imageData.nz );

    return this->imageData.values.at( compute_index(bx + accessX, by + accessY, bz + accessZ) );

  }

private:

  // compute index for values outside the domain such that we have
  // Neumann boundary conditions
  int compute_index_neumann( const int& index, const int& size ) const {

    int result = index;

    // use Neumann boundary conditions if a value outside the domain
    // is requested
    if( index < 0 || index >= size ) {

      // compute distance to boundary
      result = index % size;

      // get mirrored value
      // boundaries are mirrored, such that one-sided differences vanish
      result = ( size - 1 ) - result;

      // map back to interval
      result %= size;

    }

    return result;
  }

};
#endif
