#ifndef __FIT_MODEL_LANDMARK_TERM_H__
#define __FIT_MODEL_LANDMARK_TERM_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "model/Model.h"
#include "optimization/fitmodel/EnergyTerm.h"
#include "optimization/fitmodel/Energy.h"

namespace fitModel{

  /* class that represents the landmark term of the energy function */
  class LandmarkTerm : public EnergyTerm {

  public:

    /*--------------------------------------------------------------------------*/

    LandmarkTerm(Energy& energy) : energy(energy) {
    }

    /*--------------------------------------------------------------------------*/

    void add_energy_and_gradient(
      double& energy,
      vnl_vector<double>& gradient) const {

      const arma::vec difference =
        this->energy.derived_data().linearizedLandmarkSource -
        this->energy.derived_data().linearizedLandmarkTarget;

      // get weight for landmark term
      const double& factor =
        this->energy.derived_data().weights.at("landmarkTerm");

      // add energy
      energy += factor * arma::dot(difference, difference);

      // compute gradient
      arma::mat speakerDerivative = get_speaker_derivative();
      arma::mat phonemeDerivative = get_phoneme_derivative();

      const arma::vec speakerGradient =
        2 * factor * speakerDerivative * difference;

      const arma::vec phonemeGradient =
        2 * factor * phonemeDerivative * difference;

      for(unsigned int i = 0; i < speakerGradient.n_rows; ++i) {
        gradient[i] += speakerGradient(i);
      }

      const int offset = speakerGradient.n_rows;

      for(unsigned int i = 0; i < phonemeGradient.n_rows; ++i) {
        gradient[offset + i] += phonemeGradient(i);
      }

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    arma::mat get_speaker_derivative() const {
      return this->energy.data().model.derivative().speaker(
        this->energy.data().phonemeWeights).t();
    }

    /*--------------------------------------------------------------------------*/

    arma::mat get_phoneme_derivative() const {
      return this->energy.data().model.derivative().phoneme(
        this->energy.data().speakerWeights).t();
    }

    /*--------------------------------------------------------------------------*/

    Energy& energy;

    /*--------------------------------------------------------------------------*/

  };

}

#endif
