#ifndef __LUCAS_KANADE_ENERGY_DATA_H__
#define __LUCAS_KANADE_ENERGY_DATA_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "image/Image.h"

#include "matrix/Transformation.h"
#include "energy/TransformationEnum.h"

namespace lucasKanade{

  class EnergyData{

  public:

    // center of template region of interest
    arma::vec center;

    // current transformation parameters increment in serialized format
    vnl_vector<double> transformation;

    /*--------------------------------------------------------------------------*/

    const int transformationAmount = 6;

    /*--------------------------------------------------------------------------*/

    EnergyData(
               const Image& templateImage,
               const Image& target,
               const arma::vec& center,
               const arma::vec& radius
               )

      : templateImage(templateImage), target(target), center(center) {

      this->transformation = vnl_vector<double>(this->transformationAmount, 0.);

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

  };

}
#endif
