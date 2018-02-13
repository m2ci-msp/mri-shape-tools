#ifndef __LUCAS_KANADE_ENERGY_DERIVED_DATA__
#define __LUCAS_KANADE_ENERGY_DERIVED_DATA__

#include <vector>

#include <armadillo>

#include "energy/Transformation.h"

namespace lucasKanade{

  class EnergyDerivedData{

  public:

    /*--------------------------------------------------------------------------*/

    // current template in linearized format
    std::vector<double> templateImage;

    // current warped image in linearized format
    std::vector<double> warpedImage;

    // indicator if current location is valid
    std::vector<bool> validLocation;

    // product of image gradient and jacobian matrix of transformation
    // for each location
    std::vector<arma::vec> imageGradientTimesJacobian;

    // current summed up transformation parameters
    std::vector<double> transformation;

    // corresponding transformation matrix
    Transformation transformationMatrix;

    /*--------------------------------------------------------------------------*/

  };
}
#endif
