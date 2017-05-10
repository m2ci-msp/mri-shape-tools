#ifndef __MATCH_TEMPLATE_ENERGY_FUNCTION_H__
#define __MATCH_TEMPLATE_ENERGY_FUNCTION_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>

#include "optimization/matchtemplate/Energy.h"
#include "optimization/fitmodel/EnergyTerm.h"

namespace matchTemplate{

  // class used for minimizing the energy with ITK
  class EnergyFunction: public vnl_cost_function{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyFunction(
      Energy& energy,
      const std::vector<EnergyTerm*>& energyTerms
      ) :
      vnl_cost_function(energy.data().vertexAmount * energy.data().transformationAmount),
      energy(energy),
      energyTerms(energyTerms) {

    }

    /*--------------------------------------------------------------------------*/

    virtual void compute(
      const vnl_vector<double>&, double *f, vnl_vector<double>* g){

      // update data for new transformation
      this->energy.update().for_transformations();

      // make sure that energy and gradient are reset to 0
      *f = 0;

      const int entryAmount = this->energy.data().vertexAmount * this->energy.data().transformationAmount;

      for(int i = 0; i < entryAmount; ++i) {
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

    const std::vector<EnergyTerm*>& energyTerms;

    /*--------------------------------------------------------------------------*/

  };

}

#endif
