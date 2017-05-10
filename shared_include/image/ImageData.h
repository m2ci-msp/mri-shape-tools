#ifndef __IMAGE_DATA_H__
#define __IMAGE_DATA_H__

#include<vector>

class ImageData{

public:

  /*----------------------------------------------------------------------*/

  ImageData() {
  }

  /*----------------------------------------------------------------------*/

  ImageData(
    const int& nx,
    const int& ny,
    const int& nz,
    const int& bx = 0,
    const int& by = 0,
    const int& bz = 0
    ) :
    nx(nx), ny(ny), nz(nz),
    bx(bx), by(by), bz(bz) {

    this->values = std::vector<double>(
      ( 2 * bx + nx ) *
      ( 2 * by + ny ) *
      ( 2 * bz + nz )
      );

  }

  /*----------------------------------------------------------------------*/

  void set_core_data(
    const std::vector<double>& values
    ) {

    int indexData = 0;
    int indexCore = 0;

    for(int z = 0; z < nz; ++z) {
      for(int x = 0; x < nx; ++x) {

        indexData =
          (bz + z) * (ny + 2 * by) * ( 2 * bx + nx) +
          (bx + x) * (ny + 2 * by) +
          by;

        indexCore = z * ny * nx + x * ny;

        for(int k = 0; k < ny; ++k) {


          this->values[indexData] = values[indexCore];

          ++indexData;
          ++indexCore;

        }
      }
    }

  }

  /*----------------------------------------------------------------------*/

  void set_values(
    const std::vector<double>& values,
    const int& nx,
    const int& ny,
    const int& nz,
    const int& bx,
    const int& by,
    const int& bz
    ) {

    this->values = values;

    this->nx = nx;
    this->ny = ny;
    this->nz = nz;

    this->bx = bx;
    this->by = by;
    this->bz = bz;

  }

  /*----------------------------------------------------------------------*/

  std::vector<double>& get_values() {

    return this->values;

  }

  /*----------------------------------------------------------------------*/

  std::vector<double> get_x_slice(
    const int& y,
    const int& z
    ) const {

    int currentIndex =
      (bz + z) * ( 2 * by + this->ny ) * ( 2 * bx + this->nx) +
      0 * this->ny +
      by + y;

    const int increment =
      this->ny + 2 * by;

    std::vector<double> result(this->nx + 2 * bx, 0);

    for(int i = 0; i < 2 * bx + nx; ++i) {
      result[i] = this->values[currentIndex];
      currentIndex += increment;
    }

    return result;

  }

  /*----------------------------------------------------------------------*/

  std::vector<double> get_y_slice(
    const int& x,
    const int& z
    ) const {

    int currentIndex =
      (bz + z) * ( 2 * by + this->ny )  * (2 * bx + this->nx) +
      (bx + x) * ( 2 * by + this->ny ) +
      0;

    const int increment = 1;

    std::vector<double> result(this->ny + 2 * by, 0);

    for(int i = 0; i < 2 * by + ny; ++i) {
      result[i] = this->values[currentIndex];
      currentIndex += increment;
    }

    return result;

  }

  /*----------------------------------------------------------------------*/

  std::vector<double> get_z_slice(
    const int& x,
    const int& y
    ) const {

    int currentIndex =
      0 * this->ny * this->nx +
      (bx + x) * ( 2 * by + this->ny) +
      (by + y);

    const int increment = ( this->ny + 2 * by ) * ( this->nx + 2 * bx );

    std::vector<double> result(this->nz + 2 * bz, 0);

    for(int i = 0; i < 2 * bz + nz; ++i) {
      result[i] = this->values[currentIndex];
      currentIndex += increment;
    }

    return result;

  }


  /*----------------------------------------------------------------------*/

  double& at(
    const int& x,
    const int& y,
    const int& z
    ) {


    return this->values[
         compute_index(bx + x, by + y, bz + z)
      ];

  }

  /*----------------------------------------------------------------------*/

  void set(
    const int& x,
    const int& y,
    const int& z,
    const double& v
    ) {

    this->values[
      compute_index(bx + x, by + y, bz + z)
      ] = v;

  }

  /*----------------------------------------------------------------------*/

  const double& at(
    const int& x,
    const int& y,
    const int& z
    ) const {


    return this->values[
      compute_index(bx + x, by + y, bz + z)
      ];

  }

  /*----------------------------------------------------------------------*/


  double& at_raw(
    const int& x,
    const int& y,
    const int& z
    ) {


    return this->values[
      compute_index(x, y, z)
      ];

  }

  /*----------------------------------------------------------------------*/

  void set_boundary_sizes(
    const int& bxNew,
    const int& byNew,
    const int& bzNew
    ) {

    std::vector<double> newValues(
      ( 2 * bxNew + nx ) *
      ( 2 * byNew + ny ) *
      ( 2 * bzNew + nz )
      );


    for(int x = 0; x < this->nx; ++x) {
      for(int y = 0; y < this->ny; ++y) {
        for(int z = 0; z < this->nz; ++z) {

          const int oldIndex =
            (bz + z) * ( 2 * by + this->ny) * ( 2 * bx + this->nx) +
            (bx + x) * ( 2 * by + this->ny) +
            (by + y);

          const int newIndex =
            (bzNew + z) * ( 2 * byNew + this->ny ) * (2 * bxNew + this->nx) +
            (bxNew + x) * ( 2 * byNew + this->ny ) +
            (byNew + y);

          newValues[newIndex] = this->values[oldIndex];

        } // end for y
      } // end for x
    } // end for z

    this->bx = bxNew;
    this->by = byNew;
    this->bz = bzNew;

    this->values = newValues;

  }

  /*----------------------------------------------------------------------*/

  void mirror() {

    mirror_x();
    mirror_y();
    mirror_z();

  }

  /*----------------------------------------------------------------------*/

  void mirror_x() {

    for(int z = 0; z < this->nz; ++z) {
      for(int y = 0; y < this->ny; ++y) {
        for(int x = 0; x < this->bx; ++x) {

          this->at(     - x - 1, y, z ) = this->at(x,                y, z);
          this->at(this->nx + x, y, z ) = this->at(this->nx - 1 - x, y, z);

        } // end for x
      } // end for y
    } // end for z

  }

  /*----------------------------------------------------------------------*/

  void mirror_y() {

    for(int x = 0; x < this->nx; ++x) {
      for(int z = 0; z < this->nz; ++z) {
        for(int y = 0; y < this->by; ++y) {

          this->at(x,      - y - 1, z ) = this->at(x,                y, z);
          this->at(x, this->ny + y, z ) = this->at(x, this->ny - 1 - y, z);

        } // end for y
      } // end for z
    } // end for x

  }

  /*----------------------------------------------------------------------*/

  void mirror_z() {

    for(int x = 0; x < this->nx; ++x) {
      for(int y = 0; y < this->ny; ++y) {
        for(int z = 0; z < this->bz; ++z) {

          this->at(x, y, - z - 1      ) = this->at(x, y, z               );
          this->at(x, y, this->nz + z ) = this->at(x, y, this->nz - 1 - z);

        } // end for z
      } // end for y
    } // end for x

  }

  /*----------------------------------------------------------------------*/

  const int& get_nx() const { return this->nx; }
  const int& get_ny() const { return this->ny; }
  const int& get_nz() const { return this->nz; }

  const int& get_bx() const { return this->bx; }
  const int& get_by() const { return this->by; }
  const int& get_bz() const { return this->bz; }

  /*----------------------------------------------------------------------*/

  std::vector<double> values;

  /*----------------------------------------------------------------------*/
private:

  /*----------------------------------------------------------------------*/

  int compute_index(int x, int y, int z) const {

    return
      z * ( this->ny + 2 * by) * ( this->nx + 2 * bx) +
      x * ( this->ny + 2 * by) +
      y;

  }

  /*----------------------------------------------------------------------*/

  double hx = 1;
  double hy = 1;
  double hz = 1;

  int nx = 0;
  int ny = 0;
  int nz = 0;

  int bx = 0;
  int by = 0;
  int bz = 0;

  /*----------------------------------------------------------------------*/

};
#endif
