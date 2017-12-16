#ifndef __IMAGE_CALCULUS_H__
#define __IMAGE_CALCULUS_H__

#include <cmath>
#include <armadillo>

#include "image/ImageData.h"
#include "image/ImageAccess.h"

class ImageCalculus{

public:

  ImageCalculus(ImageData& imageData) :
    imageData(imageData),
    imageAccess(ImageAccess(imageData)) {
  }

  double derivative_x(
                      const int& x,
                      const int& y,
                      const int& z
                      ) const {
  
    const double& left  = this->imageAccess.at_grid(x    , y, z);
    const double& right = this->imageAccess.at_grid(x + 1, y, z);
  
    const double& hx = this->imageData.hx;
  
    return (right - left) / hx;
  
  }

  double derivative_y(
                      const int& x,
                      const int& y,
                      const int& z
                      ) const {
  
    const double& lower = this->imageAccess.at_grid(x, y    , z);
    const double& upper = this->imageAccess.at_grid(x, y + 1, z);
  
    const double& hy = this->imageData.hy;
  
    return (upper - lower) / hy;
  
  }

  double derivative_z(
                      const int& x,
                      const int& y,
                      const int& z
                      ) const {
  
    const double& back  = this->imageAccess.at_grid(x, y, z    );
    const double& front = this->imageAccess.at_grid(x, y, z + 1);
  
    const double& hz = this->imageData.hz;
  
    return (front - back) / hz;
  
  }

  double derivative_xx(
                       const int& x,
                       const int& y,
                       const int& z
                       ) const {
  
    const double& left   = this->imageAccess.at_grid(x-1, y, z);
    const double& center = this->imageAccess.at_grid(x  , y, z);
    const double& right  = this->imageAccess.at_grid(x+1, y, z);
  
    const double& hx = this->imageData.hx;
  
    return ( left + 2 * center + right ) / ( pow( hx, 2 ) );
  
  }

  double derivative_yy(
                       const int& x,
                       const int& y,
                       const int& z
                       ) const {
  
    const double& lower  = this->imageAccess.at_grid(x, y-1, z);
    const double& center = this->imageAccess.at_grid(x, y  , z);
    const double& upper  = this->imageAccess.at_grid(x, y+1, z);
  
    const double& hy = this->imageData.hy;
  
    return ( lower + 2 * center + upper ) / ( pow( hy, 2 ) );
  
  }

  double derivative_zz(
                       const int& x,
                       const int& y,
                       const int& z
                       ) const {
  
    const double& back   = this->imageAccess.at_grid(x, y, z-1);
    const double& center = this->imageAccess.at_grid(x, y  , z);
    const double& front  = this->imageAccess.at_grid(x, y, z+1);
  
    const double& hz = this->imageData.hz;
  
    return ( back + 2 * center + front ) / ( pow( hz, 2 ) );
  
  }

  double laplacian(
                   const int& x,
                   const int& y,
                   const int& z
                   ) const {
  
    return
      derivative_xx(x, y, z) +
      derivative_yy(x, y, z) +
      derivative_zz(x, y, z);
  
  }

  arma::vec gradient(
                     const int& x,
                     const int& y,
                     const int& z
                     ) const {
  
    return arma::vec({
        derivative_x(x, y, z ),
          derivative_y(x, y, z ),
          derivative_z(x, y, z )
          });
  
  }

  arma::mat structure_tensor(
                             const int& x,
                             const int& y,
                             const int& z
                             ) const {
  
    const arma::vec grad = gradient(x, y, z);
  
    return grad * grad.t();
  
  }

private:

  ImageData& imageData;

  const ImageAccess imageAccess;

};

#endif
