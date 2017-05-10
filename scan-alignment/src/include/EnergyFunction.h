#ifndef __ENERGY_FUNCTION_H__
#define __ENERGY_FUNCTION_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>

#include "EnergyData.h"
#include "DataTerm.h"


// class used for minimizing the energy with ITK
class EnergyFunction: public vnl_cost_function{

public:

  /*--------------------------------------------------------------------------*/

  EnergyFunction(
    Energy& energy,
    const std::vector<EnergyTerm*>& energyTerms
    ) :
    vnl_cost_function(
      energy.data().transformation.size()
      ),
    energy(energy),
    energyTerms(energyTerms) {

  }

  /*--------------------------------------------------------------------------*/

  virtual void compute(
    const vnl_vector<double>&, double *f, vnl_vector<double>* g){

    // make sure that energy and gradient are reset to 0
    *f = 0;

    for(unsigned int i = 0; i < this->energy.data().transformation.size(); ++i) {
      (*g)[i] = 0;
    }

    for(const EnergyTerm* term: this->energyTerms) {
      term->add_energy_and_gradient(*f, *g);
    }

  }

  /*--------------------------------------------------------------------------*/

  double compute_only_current_energy() {

    double result = 0;

    for(const EnergyTerm* term: this->energyTerms) {
      result += term->compute_current_energy();
    }

    return result;

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  Energy& energy;

  std::vector<EnergyTerm*> energyTerms;

  /*--------------------------------------------------------------------------*/

};


#endif
