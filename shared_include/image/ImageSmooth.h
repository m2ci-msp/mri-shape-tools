#ifndef __IMAGE_SMOOTH_H__
#define __IMAGE_SMOOTH_H__

#include "image/ImageData.h"
#include "gaussian/GaussianBuilder.h"

class ImageSmooth{

public:

  /*----------------------------------------------------------------------*/

  ImageSmooth(
    ImageData& imageData
    ) :
    data(imageData) {

  }

  /*----------------------------------------------------------------------*/

  void gaussian(
    const double& sigma
    ) {

    if( sigma == 0) {
      return;
    }

    const std::vector<double> kernel = GaussianBuilder::build(sigma, 3);

    const int radius = (kernel.size() - 1) / 2;

    if( this->data.get_bx() < radius ||
        this->data.get_by() < radius ||
        this->data.get_bz() < radius
      ) {

      this->data.set_boundary_sizes(radius, radius, radius);

    }

    convolve_along_x(kernel, radius);
    convolve_along_y(kernel, radius);
    convolve_along_z(kernel, radius);

  }

  /*----------------------------------------------------------------------*/

private:

  /*----------------------------------------------------------------------*/

  void convolve_along_x(
    const std::vector<double>& kernel,
    const int& radius
    ) {

    const int& nx = this->data.get_nx();
    const int& ny = this->data.get_ny();
    const int& nz = this->data.get_nz();

    const int& bx = this->data.get_bx();

    data.mirror_x();

    std::vector<double> result(nx * ny * nz);

    for(int z = 0; z < nz; ++z) {

      for(int y = 0; y < ny; ++y) {

        std::vector<double> slice = data.get_x_slice(y, z);

          for(int x = bx; x < bx + nx; ++x) {

            double value = 0;

            for(int r = -radius; r <= radius; ++r) {
              value += kernel[radius + r] * slice[x + r];
            }

            data.at(x - bx, y, z) = value;

          } // end for x
      } // end for y
    } // end for z

  } // end convolve_along_x

  /*----------------------------------------------------------------------*/

  void convolve_along_y(
    const std::vector<double>& kernel,
    const int& radius
    ) {

    const int& nx = this->data.get_nx();
    const int& ny = this->data.get_ny();
    const int& nz = this->data.get_nz();

    const int& by = this->data.get_by();

    data.mirror_y();

    std::vector<double> result(nx * ny * nz);

    for(int z = 0; z < nz; ++z) {
      for(int x = 0; x < nx; ++x) {

        std::vector<double> slice = data.get_y_slice(x, z);

        for(int y = by; y < by + ny; ++y) {

          double value = 0;

          for(int r = -radius; r <= radius; ++r) {
            value += kernel[radius + r] * slice[y + r];
          }

          data.at(x, y - by, z) = value;

        } // end for y
      } // end for x
    } // end for z

  } // end convolve_along_y

  /*----------------------------------------------------------------------*/

  void convolve_along_z(
    const std::vector<double>& kernel,
    const int& radius
    ) {

    const int& nx = this->data.get_nx();
    const int& ny = this->data.get_ny();
    const int& nz = this->data.get_nz();

    const int& bz = this->data.get_bz();

    data.mirror_z();

    std::vector<double> result(nx * ny * nz);

    for(int x = 0; x < nx; ++x) {
      for(int y = 0; y < ny; ++y) {

        std::vector<double> slice = data.get_z_slice(x, y);

        for(int z = bz; z < bz + nz; ++z) {

          double value = 0;

          for(int r = -radius; r <= radius; ++r) {
            value += kernel[radius + r] * slice[z + r];
          }

          data.at(x, y, z - bz) = value;

        } // end for z
      } // end for y
    } // end for x

  } // end convolve_along_z

  /*----------------------------------------------------------------------*/

  ImageData& data;

  /*----------------------------------------------------------------------*/

};

#endif
