#ifndef __FIT_MODEL_MEAN_BIAS_TERM_H__
#define __FIT_MODEL_MEAN_BIAS_TERM_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "optimization/fitmodel/Energy.h"
#include "optimization/fitmodel/EnergyTerm.h"

namespace fitModel{

  /* class that represents the mean bias term of the energy function */

  class MeanBiasTerm : public EnergyTerm {

  public:

    /*--------------------------------------------------------------------------*/

    MeanBiasTerm(Energy& energy) : energy(energy) {
    }

    /*--------------------------------------------------------------------------*/

    virtual void add_energy_and_gradient(
      double& energy,
      vnl_vector<double>& gradient) const {

      // compute differences
      const arma::vec speakerDifference =
        this->energy.data().speakerWeights -
        this->energy.data().model.data().get_speaker_mean_weights();

      const arma::vec phonemeDifference =
        this->energy.data().phonemeWeights -
        this->energy.data().model.data().get_phoneme_mean_weights();

      // get energy term weights
      const double& meanBiasWeight =
        this->energy.settings().weights.at("meanBiasTerm");

      // add energy
      energy +=
        meanBiasWeight *
        arma::dot(speakerDifference, speakerDifference);

      energy +=
        meanBiasWeight *
        arma::dot(phonemeDifference, phonemeDifference);

      // add gradient
      for(unsigned int i = 0; i < speakerDifference.n_rows; ++i) {
        gradient[i] += 2 * meanBiasWeight * speakerDifference(i);
      }

      const int offset = speakerDifference.n_rows;

      for(unsigned int i = 0; i < phonemeDifference.n_rows; ++i) {
        gradient[offset + i] += 2 * meanBiasWeight * phonemeDifference(i);
      }

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    Energy& energy;

    /*--------------------------------------------------------------------------*/

  };

}

#endif
