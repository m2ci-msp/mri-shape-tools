#ifndef __MODEL_DERIVATIVE_H__
#define __MODEL_DERIVATIVE_H__

#include <armadillo>

class ModelDerivative{

public:

  /*--------------------------------------------------------------------------*/

  ModelDerivative(const ModelSpace& modelSpace) : space(modelSpace) {
  }

  /*--------------------------------------------------------------------------*/

  // columns of returned matrix correspond to derivatives with respect to
  // the speaker weights
  arma::mat speaker(const arma::vec& phonemeWeights) const {

    return this->space.phoneme(phonemeWeights);

  }

  /*--------------------------------------------------------------------------*/

  // columns of returned matrix correspond to derivatives with respect to
  // the phoneme weights
  arma::mat phoneme(const arma::vec& speakerWeights) const {

    return this->space.speaker(speakerWeights);

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  const ModelSpace& space;

  /*--------------------------------------------------------------------------*/

};
#endif
