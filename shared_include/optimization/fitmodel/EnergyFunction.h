#ifndef __FIT_MODEL_ENERGY_FUNCTION_H__
#define __FIT_MODEL_ENERGY_FUNCTION_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>

#include "optimization/fitmodel/EnergyData.h"
#include "optimization/fitmodel/EnergySettings.h"
#include "optimization/fitmodel/DataTerm.h"
#include "optimization/fitmodel/LandmarkTerm.h"
#include "optimization/fitmodel/ITKWrapper.h"

namespace fitModel{

  // class used for minimizing the energy with ITK
  class EnergyFunction: public vnl_cost_function{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyFunction(
      Energy& energy,
      const std::vector<EnergyTerm*>& energyTerms
      ) :
      vnl_cost_function(
        energy.data().model.data().get_speaker_mode_dimension() +
        energy.data().model.data().get_phoneme_mode_dimension()
        ),
      energy(energy),
      energyTerms(energyTerms) {

    }

    /*--------------------------------------------------------------------------*/

    virtual void compute(
      const vnl_vector<double>& x, double *f, vnl_vector<double>* g){

      // set weights
      ITKWrapper::vnl_vector_to_weights(
        x,
        this->energy.data().speakerWeights,
        this->energy.data().phonemeWeights
        );

      // update data for new weights
      this->energy.update().for_weights();

      const int speakerWeightAmount = this->energy.data().speakerWeights.size();
      const int phonemeWeightAmount = this->energy.data().phonemeWeights.size();

      // make sure that energy and gradient are reset to 0
      *f = 0;

      for(int i = 0; i < speakerWeightAmount + phonemeWeightAmount; ++i) {
        (*g)[i] = 0;
      }

      for(const EnergyTerm* term: this->energyTerms) {
        term->add_energy_and_gradient(*f, *g);
      }

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    Energy& energy;

    std::vector<EnergyTerm*> energyTerms;

    /*--------------------------------------------------------------------------*/

  };

}

#endif
