#ifndef __RIGID_ALIGNMENT_ENERGY_FUNCTION_H__
#define __RIGID_ALIGNMENT_ENERGY_FUNCTION_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>

#include "optimization/rigidalignment/EnergyData.h"
#include "optimization/rigidalignment/EnergySettings.h"
#include "optimization/rigidalignment/DataTerm.h"
#include "optimization/rigidalignment/ITKWrapper.h"

namespace rigidAlignment{

  // class used for minimizing the energy with ITK
  class EnergyFunction: public vnl_cost_function{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyFunction(
      Energy& energy,
      const std::vector<EnergyTerm*>& energyTerms
      ) :
      vnl_cost_function(energy.data().transformationAmount),
      energy(energy),
      energyTerms(energyTerms) {

    }

    /*--------------------------------------------------------------------------*/

    virtual void compute(
      const vnl_vector<double>& x, double *f, vnl_vector<double>* g){

      // set transformation
      ITKWrapper::vnl_vector_to_transformation(
        x,
        this->energy.data().transformationMatrix,
        this->energy.data().source
        );

      // update data for new transformation
      this->energy.update().for_transformation();

      // make sure that energy and gradient are reset to 0
      *f = 0;

      for(int i = 0; i < this->energy.data().transformationAmount; ++i) {
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
