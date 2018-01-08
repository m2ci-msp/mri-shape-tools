#ifndef __IMAGE_CALCULUS_H__
#define __IMAGE_CALCULUS_H__

#include <cmath>
#include <armadillo>

#include "image/ImageData.h"
#include "image/ImageAccess.h"

class ImageCalculus{

public:

  // constructor that initializes the ImageData reference
  // and creates the ImageAccess object
  ImageCalculus(ImageData& imageData) :
    imageData(imageData),
    imageAccess(ImageAccess(imageData)) {
  }

  double dx(
            const int& x,
            const int& y,
            const int& z
            ) const {
  
    const double& left  = this->imageAccess.at_grid(x    , y, z);
    const double& right = this->imageAccess.at_grid(x + 1, y, z);
  
    const double& hx = this->imageData.hx;
  
    return (right - left) / hx;
  
  }
  
  double dy(
            const int& x,
            const int& y,
            const int& z
            ) const {
  
    const double& lower = this->imageAccess.at_grid(x, y    , z);
    const double& upper = this->imageAccess.at_grid(x, y + 1, z);
  
    const double& hy = this->imageData.hy;
  
    return (upper - lower) / hy;
  
  }
  
  double dz(
            const int& x,
            const int& y,
            const int& z
            ) const {
  
    const double& back  = this->imageAccess.at_grid(x, y, z    );
    const double& front = this->imageAccess.at_grid(x, y, z + 1);
  
    const double& hz = this->imageData.hz;
  
    return (front - back) / hz;
  
  }

  double dxx(
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
  
  double dyy(
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
  
  double dzz(
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
      dxx(x, y, z) +
      dyy(x, y, z) +
      dzz(x, y, z);
  
  }

  arma::vec gradient(
                     const int& x,
                     const int& y,
                     const int& z
                     ) const {
  
    return arma::vec({
        dx(x, y, z ),
          dy(x, y, z ),
          dz(x, y, z )
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

  arma::mat region_structure_tensor(
                                    const int& x,
                                    const int& y,
                                    const int& z,
                                    const int& radius
                                    ) {

    arma::mat result = arma::zeros(3, 3);

    for(int i = -radius; i <= radius; ++i) {
      for(int j = -radius; j <= radius; ++j) {
        for(int k = -radius; k <= radius; ++k) {

          result += structure_tensor(x + radius, y + radius, z + radius);
        }
      }
    }

    return result / pow(2 * radius + 1, 2);

  }

  private:

  ImageData& imageData;

  const ImageAccess imageAccess;

};

#endif
