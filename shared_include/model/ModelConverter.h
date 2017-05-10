#ifndef __MODEL_CONVERTER_H__
#define __MODEL_CONVERTER_H__

#include <cmath>

#include <armadillo>

#include "model/ModelData.h"

class ModelConverter{

public:

  /*--------------------------------------------------------------------------*/

  ModelConverter(const ModelData& modelData): modelData(modelData) {

    this->scaleFactorSpeaker = sqrt(
      modelData.get_original_speaker_mode_dimension()
      );

    this->scaleFactorPhoneme = sqrt(
      modelData.get_original_phoneme_mode_dimension()
      );

  }

  /*--------------------------------------------------------------------------*/

  void to_weights(
    arma::vec& speakerVariations,
    arma::vec& phonemeVariations) const {

    speakerVariations /= scaleFactorSpeaker;
    speakerVariations += this->modelData.get_speaker_mean_weights();

    phonemeVariations /= scaleFactorPhoneme;
    phonemeVariations += this->modelData.get_phoneme_mean_weights();

  }

  /*--------------------------------------------------------------------------*/

  void to_variations(
    arma::vec& speakerWeights,
    arma::vec& phonemeWeights
    ) const {

    speakerWeights -= this->modelData.get_speaker_mean_weights();
    speakerWeights *= scaleFactorSpeaker;

    phonemeWeights -= this->modelData.get_phoneme_mean_weights();
    phonemeWeights *= scaleFactorPhoneme;

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  const ModelData& modelData;
  double scaleFactorSpeaker;
  double scaleFactorPhoneme;

  /*--------------------------------------------------------------------------*/
};
#endif
