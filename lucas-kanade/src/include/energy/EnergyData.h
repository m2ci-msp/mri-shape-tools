#ifndef __LUCAS_KANADE_ENERGY_DATA_H__
#define __LUCAS_KANADE_ENERGY_DATA_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "image/Image.h"

#include "matrix/Transformation.h"

#include "DeformedTemplate.h"
#include "IncrementallyDeformedTemplate.h"

namespace lucasKanade{

  class EnergyData{

  public:

    // center of template region of interest
    const arma::vec center;

    // original normalized color values of the undeformed template in the source image
    const std::vector<double> originalNormalizedValues;

    // current transformation parameters increment in serialized format
    vnl_vector<double> transformationIncrement;

    // current total transformation
    std::vector<double> transformation;

    // data structures of currently deformed template
    DeformedTemplate deformedTemplate;

    // data structures of incrementally deformed template
    IncrementallyDeformedTemplate incrementallyDeformedTemplate;

    // current transformationMatrix
    Transformation transformationMatrix;

    const int transformationAmount = 6;

    /*--------------------------------------------------------------------------*/

    EnergyData(
               const arma::vec& center,
               const std::vector<double>& originalNormalizedValues,
               const std::vector<arma::vec>& originalLocations,
               const Image& target
               )

      :
      center(center), originalNormalizedValues(originalNormalizedValues),
      deformedTemplate(target, originalLocations),
      incrementallyDeformedTemplate(target, originalLocations, deformedTemplate)

    {

      this->transformationIncrement = vnl_vector<double>(this->transformationAmount, 0.);

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

  };

}
#endif
