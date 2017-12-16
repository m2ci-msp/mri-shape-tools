#ifndef __GAUSSIAN_FILTER_H__
#define __GAUSSIAN_FILTER_H__

#include <cmath>

#include "image/ImageAccess.h"
#include "image/ImageBoundary.h"
#include "image/ImageData.h"
#include "image/ImageMirror.h"

class GaussianFilter{

public:

  GaussianFilter(
                 ImageData& imageData,
                 const double& stdDev
    ) :
    imageData(imageData),
    imageAccess(ImageAccess(imageData)),
    imageBoundary(ImageBoundary(imageData)),
    imageMirror(ImageMirror(imageData)),
    stdDev(stdDev) {

    init_gauss_kernel();

    adjust_image_boundaries();

  }

  void apply() {

    if( this->stdDev == 0) {
      return;
    }

    convolve_along_x();
    convolve_along_y();
    convolve_along_z();

  }

private:

  void convolve_along_x() {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    const int& bx = this->imageData.bx;

    this->imageMirror.x();

    for(int z = 0; z < nz; ++z) {

      for(int y = 0; y < ny; ++y) {

        const std::vector<double> row = this->imageAccess.x_row_at(y, z);

          for(int x = bx; x < bx + nx; ++x) {

            double value = 0;

            for(int r = -this->radius; r <= this->radius; ++r) {
              value += this->kernel[this->radius + r] * row[x + r];
            }

            this->imageAccess.at_grid(x - bx, y, z) = value;

          } // end for x
      } // end for y
    } // end for z

  } // end convolve_along_x

  void convolve_along_y() {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    const int& by = this->imageData.by;

    this->imageMirror.y();

    for(int z = 0; z < nz; ++z) {
      for(int x = 0; x < nx; ++x) {

        const std::vector<double> row = this->imageAccess.y_row_at(x, z);

        for(int y = by; y < by + ny; ++y) {

          double value = 0;

          for(int r = -this->radius; r <= this->radius; ++r) {
            value += this->kernel[this->radius + r] * row[y + r];
          }

          this->imageAccess.at_grid(x, y - by, z) = value;

        } // end for y
      } // end for x
    } // end for z

  } // end convolve_along_y

  void convolve_along_z() {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    const int& bz = this->imageData.bz;

    this->imageMirror.z();

    for(int x = 0; x < nx; ++x) {
      for(int y = 0; y < ny; ++y) {

        const std::vector<double> row = this->imageAccess.z_row_at(x, y);

        for(int z = bz; z < bz + nz; ++z) {

          double value = 0;

          for(int r = -this->radius; r <= this->radius; ++r) {
            value += kernel[this->radius + r] * row.at(z + r);
          }

          this->imageAccess.at_grid(x, y, z - bz) = value;

        } // end for z
      } // end for y
    } // end for x

  } // end convolve_along_z

  void init_gauss_kernel() {

      // radius of truncated gaussian to each side in terms
      // of standard deviation
      // we use a radius of 3 stdDev
      const int size = 3;

      // ensure that we have at least radius 1
      this->radius = ( size * this->stdDev > 0) ? size * this->stdDev: 1;

      // resize kernel
      this->kernel.resize(2 * radius + 1, 0);

      // time savers
      const double ts1 = 2. * pow( this->stdDev, 2 );
      const double ts2 = 1. / sqrt(ts1 * M_PI);

      double sum = 0;

      for(int i = 0; i < 2 * this->radius + 1; ++i) {

      // center Gaussian at radius
      const double x = pow(i - this->radius, 2.);

      kernel[i] = ts2 * exp( - x / ts1 );

      sum += this->kernel[i];

      } // end for

      // normalize kernel like in code by J. Weickert
      // -> ensure that entries of truncated kernel sum up to 1
      for(double& value: kernel) {
          value /= sum;
      }

  }

  void adjust_image_boundaries() {

    if( this->imageData.bx < this->radius ||
        this->imageData.by < this->radius ||
        this->imageData.bz < this->radius
      ) {

      this->imageBoundary.change_boundary(radius, radius, radius);

    }

  }

  ImageData& imageData;

  ImageAccess imageAccess;

  ImageBoundary imageBoundary;

  ImageMirror imageMirror;

  const double stdDev;

  int radius;

  std::vector<double> kernel;

};

#endif
