#ifndef __RADIAL_BASIS_H__
#define __RADIAL_BASIS_H__

#include <cmath>

class RadialBasis{

public:

  static double compute(const double& x) {

    const double result = ( x <= 0 ) ?
      0 :
      pow(x, 2) * log(x          ) ;

    return result;

  }

private:

  RadialBasis();

};

#endif
