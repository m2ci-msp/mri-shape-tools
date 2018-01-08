#ifndef __IMAGE_ACCESS_H__
#define __IMAGE_ACCESS_H__

#include "image/ImageData.h"
#include "image/ImageCoordinate.h"

class ImageAccess{

public:

  // constructor that initializes the ImageData reference
  ImageAccess(ImageData& imageData) : imageData(imageData) {
  }

  double& at_grid(
                  const int& x,
                  const int& y,
                  const int& z
                  ) {
  
    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;
  
    return this->imageData.values.at( compute_index(bx + x, by + y, bz + z) );
  
  }
  
  // const version
  const double& at_grid(
                        const int& x,
                        const int& y,
                        const int& z
                        ) const {
  
    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;
  
    return this->imageData.values.at( compute_index(bx + x, by + y, bz + z) );
  
  }

  double& at_coordinate(const double& x, const double& y, const double& z) {

    int i, j, k;

    ImageCoordinate(this->imageData).to_grid(x, y, z, i, j, k);

    return at_grid(i, j, k);

  }

  // const version
  const double& at_coordinate(const double& x, const double& y, const double& z) const {

    int i, j, k;

    ImageCoordinate(this->imageData).to_grid(x, y, z, i, j, k);

    return at_grid(i, j, k);

  }

  double& at_grid_raw(
                      const int& x,
                      const int& y,
                      const int& z
                      ) {
  
    return this->imageData.values.at( compute_index(x, y, z) );
  
  }
  
  // const version
  const double& at_grid_raw(
                            const int& x,
                            const int& y,
                            const int& z
                            ) const {
  
    return this->imageData.values.at( compute_index(x, y, z) );
  
  }

  std::vector<double> x_row_at(
                               const int& y,
                               const int& z
                               ) const {
  
    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
  
    // compute start position of row
    int currentIndex = compute_index(0, y + by, z + bz);
  
    // we extract the whole row including the boundary
    const int increment = ny + 2 * by;
  
    std::vector<double> result(nx + 2 * bx, 0);
  
    for(int i = 0; i < 2 * bx + nx; ++i) {
  
      result[i] = this->imageData.values.at(currentIndex);
      currentIndex += increment;
  
    }
  
    return result;
  
  }
  

  std::vector<double> y_row_at(
                                 const int& x,
                                 const int& z
                                 ) const {
  
      const int& bx = this->imageData.bx;
      const int& by = this->imageData.by;
      const int& bz = this->imageData.bz;
  
      const int& ny = this->imageData.ny;
  
      // compute start position of row
      int currentIndex = compute_index(x + bx, 0, z + bz);
  
      // we extract the whole row including the boundary
      const int increment = 1;
  
      std::vector<double> result(ny + 2 * by, 0);
  
      for(int i = 0; i < 2 * by + ny; ++i) {
        result[i] = this->imageData.values.at(currentIndex);
        currentIndex += increment;
      }
  
      return result;
  
    }

  std::vector<double> z_row_at(
                               const int& x,
                               const int& y
                               ) const {
  
    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;
  
    // compute start position of row
    int currentIndex = compute_index(x + bx, y + by, 0);
  
    // we extract whole row including the boundary
    const int increment = ( ny + 2 * by ) * ( nx + 2 * bx );
  
    std::vector<double> result(nz + 2 * bz, 0);
  
    for(int i = 0; i < 2 * bz + nz; ++i) {
      result[i] = this->imageData.values.at(currentIndex);
      currentIndex += increment;
    }
  
    return result;
  
  }

private:

  int compute_index(
                    const int& x,
                    const int& y,
                    const int& z
                    ) const {
  
    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
  
    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
  
    const int index =
      z * ( 2 * by + ny ) * ( 2 * bx + nx ) +
      x * ( 2 * by + ny ) +
      y;
  
    return index;
  
  }

  ImageData& imageData;

};
#endif
