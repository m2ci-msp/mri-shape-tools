#ifndef __GAUSSIAN_BUILDER_H__
#define __GAUSSIAN_BUILDER_H__

#include <cmath>

class GaussianBuilder{

public:

  /*--------------------------------------------------------------------------*/

  static std::vector<double> build(
    // standard deviation
    const double& sigma,
    // radius of truncated gaussian to each side in terms
    // of standard deviation
    const int& size
    ) {

    // ensure that we have at least radius 1
    const int radius = ( size * sigma > 0) ? size * sigma: 1;

    std::vector<double> kernel(2 * radius + 1, 0);

    // time savers
    const double ts1 = 2. * sigma * sigma;
    const double ts2 = 1. / sqrt(ts1 * M_PI);

    double sum = 0;

    for(int i = 0; i < 2 * radius + 1; ++i) {

      // center Gaussian at radius
      const double x = pow(i - radius, 2.);

      kernel[i] = ts2 * exp( - x / ts1 );

      sum += kernel[i];

    } // end for

    // normalize kernel like in code by J. Weickert
    // -> ensure that entries of truncated kernel sum up to 1
    for(double& value: kernel) {
      value /= sum;
    }

    return kernel;

  } // end build

  /*--------------------------------------------------------------------------*/

};
#endif
