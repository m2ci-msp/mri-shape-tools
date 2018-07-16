#ifndef __SPHERE_LEVEL_SET_BUILDER_H__
#define __SPHERE_LEVEL_SET_BUILDER_H__

#include <armadillo>

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageCreate.h"

class SphereLevelSetBuilder{

private:

  int nx;
  int ny;
  int nz;

  double hx;
  double hy;
  double hz;

  arma::vec center;
  double radius;

  bool dimensionsSet = false;
  bool spacingsSet = false;
  bool centerSet = false;
  bool radiusSet = false;

public:

  SphereLevelSetBuilder& with_dimensions(
                                  const int& nx,
                                  const int& ny,
                                  const int& nz) {

    this->nx = nx;
    this->ny = ny;
    this->nz = nz;

    this->dimensionsSet = true;

    return *this;

  }

  SphereLevelSetBuilder& with_grid_spacings(
                                     const double& hx,
                                     const double& hy,
                                     const double& hz) {

    this->hx = hx;
    this->hy = hy;
    this->hz = hz;

    this->spacingsSet = true;

    return *this;

  }

  SphereLevelSetBuilder& with_center(const arma::vec& center) {

    this->center = center;

    this->centerSet = true;

    return *this;

  }

  SphereLevelSetBuilder& with_radius(const double& radius) {

    this->radius = radius;

    this->radiusSet = true;

    return *this;

  }

  ImageData build() {

    if( valid() == false ) {

      throw std::logic_error("Necessary data for SphereLevelSetBuilder was not provided.");

    }

    return create_sphere();

  }

private:

  ImageData create_sphere() {

    ImageData result;

    ImageCreate(result)
      .with_dimension(this->nx, this->ny, this->nz)
      .with_boundary(0, 0, 0)
      .with_grid_spacing(this->hx, this->hy, this->hz)
      .empty_image();

    ImageAccess access(result);

    for(int i = 0; i < this->nx; ++i) {

      for(int j = 0; j < this->ny; ++j) {

        for(int k = 0; k < this->nz; ++k) {

          const arma::vec point({(double) i, (double) j, (double) k});

          const double distance = arma::norm(point - this->center);

          access.at_grid(i, j, k) = - distance + this->radius;

        } // end for i

      } // end for j

    } // end for k

    return result;

  } // end for create_sphere

  bool valid() const {

    return
      this->dimensionsSet &&
      this->spacingsSet &&
      this->centerSet &&
      this->radiusSet;

  }

};
#endif
