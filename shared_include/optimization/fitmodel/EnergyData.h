#ifndef __FIT_MODEL_ENERGY_DATA_H__
#define __FIT_MODEL_ENERGY_DATA_H__

#include <vector>

#include <armadillo>

#include "landmark/Landmark.h"

#include "model/Model.h"
#include "mesh/Mesh.h"

namespace fitModel{

/* class that contains data that is set directly and
 * does not need to be derived
 */

  class EnergyData{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyData(const Model& model, const Mesh& target):
      model(model), target(target) {

      // initialize the weights to the mean weights
      this->speakerWeights = arma::vec(
        model.data().get_speaker_mode_dimension(),
        arma::fill::zeros);

      this->phonemeWeights = arma::vec(
        model.data().get_phoneme_mode_dimension(),
        arma::fill::zeros);

      this->model.convert().to_weights(
        this->speakerWeights, this->phonemeWeights);

      this->oldSpeakerWeights = this->speakerWeights;
      this->oldPhonemeWeights = this->phonemeWeights;

    }

    /*--------------------------------------------------------------------------*/

    EnergyData(const Model& model):
      model(model) {

      update();

    }

    /*--------------------------------------------------------------------------*/

    void update() {

      // initialize the weights to the mean weights
      this->speakerWeights = arma::vec(
        model.data().get_speaker_mode_dimension(),
        arma::fill::zeros);

      this->phonemeWeights = arma::vec(
        model.data().get_phoneme_mode_dimension(),
        arma::fill::zeros);

      this->model.convert().to_weights(
        this->speakerWeights, this->phonemeWeights);

      this->oldSpeakerWeights = this->speakerWeights;
      this->oldPhonemeWeights = this->phonemeWeights;

    }

    /*--------------------------------------------------------------------------*/

    const Model& model;

    // current weights
    arma::vec speakerWeights;
    arma::vec phonemeWeights;

    // old weights for evaluating the smoothness in a tracking scenario
    arma::vec oldSpeakerWeights;
    arma::vec oldPhonemeWeights;

    Mesh target;

    std::vector<Landmark> landmarks;

    /*--------------------------------------------------------------------------*/

  };
}

#endif
