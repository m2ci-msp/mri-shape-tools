#ifndef __IMAGE_INTERPOLATE_H__
#define __IMAGE_INTERPOLATE_H__

#include "image/ImageAccess.h"

class ImageInterpolate{

public:

  ImageInterpolate(const ImageAccess& imageAccess) : imageAccess(imageAccess) {
  }

  double at_grid(
                 const double& x,
                 const double& y,
                 const double& z
                 ) const {
  
    // cast to integer
    const int xInt = (int) x;
    const int yInt = (int) y;
    const int zInt = (int) z;
  
    // compute sub integer coordinate parts
    const double xSub = x - xInt;
    const double ySub = y - yInt;
    const double zSub = z - zInt;
  
    // get values for the participating voxels
    const double x0y0z0 = this->imageAccess.at_grid(xInt, yInt, zInt);
  
    const double xpy0z0 = this->imageAccess.at_grid(xInt + 1, yInt    , zInt     );
    const double x0ypz0 = this->imageAccess.at_grid(xInt    , yInt + 1, zInt     );
    const double x0y0zp = this->imageAccess.at_grid(xInt    , yInt    , zInt + 1 );
  
    const double x0ypzp = this->imageAccess.at_grid(xInt    , yInt + 1, zInt + 1 );
  
    const double xpy0zp = this->imageAccess.at_grid(xInt + 1, yInt    , zInt + 1 );
    const double xpypzp = this->imageAccess.at_grid(xInt + 1, yInt + 1, zInt + 1 );
  
    const double xpypz0 = this->imageAccess.at_grid(xInt + 1, yInt + 1, zInt     );
  
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

private:

  const ImageAccess& imageAccess;

};

#endif
