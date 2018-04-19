#ifndef __IMAGE_INTERPOLATE_SLICE_H__
#define __IMAGE_INTERPOLATE_SLICE_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageCreate.h"

class ImageInterpolateSlice{

private:

  ImageData& imageData;

  void change_dimensions(const int& nx, const int& ny, const int& nz) {

    ImageCreate(this->imageData)
      .with_boundary(this->imageData.bx, this->imageData.by, this->imageData.bz)
      .with_dimension(nx, ny, nz)
      .with_grid_spacing(this->imageData.hx, this->imageData.hy, this->imageData.hz)
      .with_origin(this->imageData.originX, this->imageData.originY, this->imageData.originZ)
      .empty_image();

  }

public:

  ImageInterpolateSlice(ImageData& imageData) : imageData(imageData) {
  }

  void at_xy(const double& position) {

    ImageData original = this->imageData;
    ImageInterpolate originalInterpolate(original);
    ImageAccess originalAccess(original);
    ImageBoundary(original).change(1, 1, 1);
    ImageMirror(original).all();

    ImageAccess imageAccess(this->imageData);

    const double& nx = original.nx;
    const double& ny = original.ny;
    const double& nz = original.nz;

    change_dimensions(this->imageData.nx, this->imageData.ny, this->imageData.nz + 1);

    for(int i = 0; i < nx; ++i) {

      for(int j = 0; j < ny; ++j) {

        for(int k = 0; k <= (int) position; ++k) {

          imageAccess.at_grid(i, j, k) = originalAccess.at_grid(i, j, k);

        } // end for k

        imageAccess.at_grid(i, j, (int) position + 1) = originalInterpolate.at_grid(i, j, position);

        for(int k = (int) position + 2; k < nz + 1; ++k) {

          imageAccess.at_grid(i, j, k) = originalAccess.at_grid(i, j, k - 1);

        } // end for k

      } // end for j

    } // end for i

  } // end xy_slice

  void at_xz(const double& position) {

    ImageData original = this->imageData;
    ImageInterpolate originalInterpolate(original);
    ImageAccess originalAccess(original);
    ImageBoundary(original).change(1, 1, 1);
    ImageMirror(original).all();

    ImageAccess imageAccess(this->imageData);

    const double& nx = original.nx;
    const double& ny = original.ny;
    const double& nz = original.nz;

    change_dimensions(this->imageData.nx, this->imageData.ny + 1, this->imageData.nz);

    for(int i = 0; i < nx; ++i) {

      for(int k = 0; k < nz; ++k) {

        for(int j = 0; j <= (int) position; ++j) {

          imageAccess.at_grid(i, j, k) = originalAccess.at_grid(i, j, k);

        } // end for j

        imageAccess.at_grid(i, (int) position + 1, k) = originalInterpolate.at_grid(i, position, k);

        for(int j = (int) position + 2; j < ny + 1; ++j) {

          imageAccess.at_grid(i, j, k) = originalAccess.at_grid(i, j - 1, k);

        } // end for j

      } // end for k

    } // end for i

  } // end xz_slice

  void at_yz(const double& position) {

    ImageData original = this->imageData;
    ImageInterpolate originalInterpolate(original);
    ImageAccess originalAccess(original);
    ImageBoundary(original).change(1, 1, 1);
    ImageMirror(original).all();

    ImageAccess imageAccess(this->imageData);

    const double& nx = original.nx;
    const double& ny = original.ny;
    const double& nz = original.nz;

    change_dimensions(this->imageData.nx + 1, this->imageData.ny, this->imageData.nz);

    for(int j = 0; j < ny; ++j) {

      for(int k = 0; k < nz; ++k) {

        for(int i = 0; i <= (int) position; ++i) {

          imageAccess.at_grid(i, j, k) = originalAccess.at_grid(i, j, k);

        } // end for i

        imageAccess.at_grid((int) position + 1, j, k) = originalInterpolate.at_grid(position, j, k);

        for(int i = (int) position + 2; i < nx + 1; ++i) {

          imageAccess.at_grid(i, j, k) = originalAccess.at_grid(i - 1, j, k);

        } // end for i

      } // end for k

    } // end for j

  } // end yz_slice

};
#endif
