#ifndef __MODEL_RECONSTRUCTOR_H__
#define __MODEL_RECONSTRUCTOR_H__

#include <vector>

#include <armadillo>

#include "model/ModelData.h"
#include "model/ModelConverter.h"
#include "model/ModelSpace.h"

class ModelReconstructor{

public:

  /*--------------------------------------------------------------------------*/

  ModelReconstructor(
    const ModelData& modelData,
    const ModelConverter& modelConverter,
    const ModelSpace& modelSpace) :
    modelData(modelData), convert(modelConverter), space(modelSpace)  {
  }

  /*--------------------------------------------------------------------------*/

  arma::vec for_weights(
    const arma::vec& speakerWeights,
    const arma::vec& phonemeWeights
    ) const {

    const arma::mat speaker =
      this->space.speaker(speakerWeights);

    const arma::vec& origin = this->modelData.get_shape_space_origin();

    const arma::vec result =
      speaker * phonemeWeights + origin;

    return result;

  }

  /*--------------------------------------------------------------------------*/

  arma::vec for_variations(
    arma::vec speakerVariations,
    arma::vec phonemeVariations
    ) const {

    this->convert.to_weights(speakerVariations, phonemeVariations);


    return this->for_weights(speakerVariations, phonemeVariations);

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  const ModelData& modelData;
  const ModelConverter& convert;
  const ModelSpace& space;

  /*--------------------------------------------------------------------------*/
};

#endif
