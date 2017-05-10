#ifndef __SCAN_ACCESS_H__
#define __SCAN_ACCESS_H__

#include <vector>
#include <armadillo>
#include "scan/ScanData.h"

/* Class for accessing the MR values of a scan */
class ScanAccess {

  public:

    /*----------------------------------------------------------------------*/

    ScanAccess(ScanData& data) :
        values(data.get_values()),
        nx(data.get_nx()),
        ny(data.get_ny()),
        nz(data.get_nz()),
        hx(data.get_hx()),
        hy(data.get_hy()),
        hz(data.get_hz()) {
    }

    /*----------------------------------------------------------------------*/

  double get_value_index_interpolated(
    const double& x,
    const double& y,
    const double& z
    ) const {

    const int xInt = (int) x;
    const int yInt = (int) y;
    const int zInt = (int) z;

    // sub integer coordinate parts
    const double xSub = x - xInt;
    const double ySub = y - yInt;
    const double zSub = z - zInt;

    // get values for the participating voxels
    const double x0y0z0 = get_value_index(xInt, yInt, zInt);

    const double xpy0z0 = get_value_index_neumann(xInt + 1, yInt    , zInt     );
    const double x0ypz0 = get_value_index_neumann(xInt    , yInt + 1, zInt     );
    const double x0y0zp = get_value_index_neumann(xInt    , yInt    , zInt + 1 );

    const double x0ypzp = get_value_index_neumann(xInt    , yInt + 1, zInt + 1 );

    const double xpy0zp = get_value_index_neumann(xInt + 1, yInt    , zInt + 1 );
    const double xpypzp = get_value_index_neumann(xInt + 1, yInt + 1, zInt + 1 );

    const double xpypz0 = get_value_index_neumann(xInt + 1, yInt + 1, zInt     );

    // immediate results for interpolation along z-axis
    const double x0y0 = x0y0z0 + zSub * ( x0y0zp - x0y0z0 );
    const double x0yp = x0ypz0 + zSub * ( x0ypzp - x0ypz0 );

    const double xpy0 = xpy0z0 + zSub * ( xpy0zp - xpy0z0 );
    const double xpyp = xpypz0 + zSub * ( xpypzp - xpypz0 );

    // interpolate along y-axis
    const double x0 = x0y0 + ySub * ( x0yp - x0y0 );
    const double xp = xpy0 + ySub * ( xpyp - xpy0 );

    // interpolate along x-axis
    const double result = x0 + xSub * ( xp - x0 );

    return result;

  }

    /*----------------------------------------------------------------------*/

    /* accesses the MR value at the specified coordinates */
    double get_value_coordinate(
      const double& x,
      const double& y,
      const double& z) const {

      // convert coordinates to voxel indices
      const int indexX = x / this->hx;
      const int indexY = y / this->hy;
      const int indexZ = z / this->hz;

      return get_value_index(
        indexX,
        indexY,
        indexZ
        );

    }

    /*----------------------------------------------------------------------*/

    double get_value_coordinate( const arma::vec& coordinate) const {
      return get_value_coordinate(
        coordinate(0), coordinate(1), coordinate(2));
    }

    /*----------------------------------------------------------------------*/

    /* accesses the MR value at the specified voxel indices */
    double get_value_index(
      const int& x,
      const int& y,
      const int& z) const {

      // compute index to access value in serialized container
      const int index = compute_index(x, y, z);

      return this->values.at(index);
    }

    /*----------------------------------------------------------------------*/

    // get value and use Neumann boundary conditions for values outside the
    // domain
    double get_value_index_neumann(
      const int& x,
      const int& y,
      const int& z
      ) const {

      int accessX = compute_index_neumann( x, this->nx );
      int accessY = compute_index_neumann( y, this->ny );
      int accessZ = compute_index_neumann( z, this->nz );

      return get_value_index(accessX, accessY, accessZ);

    }

    /*----------------------------------------------------------------------*/

    // get value and use Neumann boundary conditions for values outside the
    // domain
    double get_value_coordinate_neumann(
      const arma::vec& coordinate
      ) const {

      // convert coordinate to voxel indices
      const int indexX = coordinate(0) / this->hx;
      const int indexY = coordinate(1) / this->hy;
      const int indexZ = coordinate(2) / this->hz;

      return get_value_index_neumann(indexX, indexY, indexZ);

    }

    /*----------------------------------------------------------------------*/

    /* changes the MR value at the specified voxel indices */
    void set_value_index(
      const int& x,
      const int& y,
      const int& z,
      const double value) {

      // compute index to access value in serialized container
      const int index = compute_index(x, y, z);

      this->values.at(index) = value;

      return;
    }

    /*----------------------------------------------------------------------*/

    /* accesses the MR value at the specified coordinates */
    void set_value_coordinate(
      const double& x,
      const double& y,
      const double& z,
      const double& value) {

      // convert coordinates to voxel indices
      const int indexX = x / this->hx;
      const int indexY = y / this->hy;
      const int indexZ = z / this->hz;

      set_value_index(
        indexX,
        indexY,
        indexZ,
        value
        );

    }

    /*----------------------------------------------------------------------*/

    void set_value_coordinate(
      const arma::vec& coordinate,
      const double& value) {

      set_value_coordinate(
        coordinate(0), coordinate(1), coordinate(2),
        value);
    }

    /*----------------------------------------------------------------------*/


  private:

    /*----------------------------------------------------------------------*/

    int compute_index(double x, double y, double z) const {

      // avoid going outside the domain
      x = ( x <  0       )? 0           : x;
      x = ( x >= this->nx)? this->nx - 1: x;

      y = ( y <  0       )? 0           : y;
      y = ( y >= this->ny)? this->ny - 1: y;

      z = ( z <  0       )? 0           : z;
      z = ( z >= this->nz)? this->nz - 1: z;

      return
        z * this->ny * this->nx +
        x * this->ny +
        y;
    }

    /*----------------------------------------------------------------------*/

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

    /*----------------------------------------------------------------------*/

    std::vector<double>& values;

    // dimensions
    const int& nx;
    const int& ny;
    const int& nz;

    // spacings
    const double& hx;
    const double& hy;
    const double& hz;

};
#endif
