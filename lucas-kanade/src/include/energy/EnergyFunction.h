#ifndef __LUCAS_KANADE_ENERGY_FUNCTION_H__
#define __LUCAS_KANADE_ENERGY_FUNCTION_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>

#include "energy/EnergyData.h"
#include "energy/ZeroNormalizedCrossCorrelation.h"

namespace lucasKanade{

  // class used for minimizing the energy with ITK
  class EnergyFunction: public vnl_cost_function{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyFunction(
                   Energy& energy
                   ) :
      vnl_cost_function(energy.data().transformationAmount),
      energy(energy) {

    }

    /*--------------------------------------------------------------------------*/

    virtual void compute(
                         const vnl_vector<double>&, double *f, vnl_vector<double>* g){

      // make sure that energy and gradient are reset to 0
      *f = 0;

      for(int i = 0; i < this->energy.data().transformationAmount; ++i) {
        (*g)[i] = 0;
      }

      add_energy_and_gradient(*f, *g);

    }

    /*--------------------------------------------------------------------------*/

  private:

    void add_energy_and_gradient(
                                 double& energy,
                                 vnl_vector<double>& gradient) const {

      const int& transformationAmount = this->energy.data().transformationAmount;


      IncrementallyDeformedTemplate& incrementallyDeformedTemplate =
        this->energy.data().incrementallyDeformedTemplate;

      const arma::vec increment( {
          this->energy.data().transformation[TX],
            this->energy.data().transformation[TY],
            this->energy.data().transformation[TZ],
            this->energy.data().transformation[ALPHA],
            this->energy.data().transformation[BETA],
            this->energy.data().transformation[GAMMA]
            }
        );

      incrementallyDeformedTemplate.compute(increment);

      ZeroNormalizedCrossCorrelation zeroNormalizedCrossCorrelation(
                                     this->energy.data().originalNormalizedValues,
                                     this->energy.data().deformedTemplate,
                                     incrementallyDeformedTemplate
                                     );

      energy = - zeroNormalizedCrossCorrelation.get_correlation();

      const arma::vec localGradient = - zeroNormalizedCrossCorrelation.get_correlation_derivative();

      for(int j = 0; j < transformationAmount; ++j) {

        gradient[j] = localGradient(j);

      }

    }

    /*--------------------------------------------------------------------------*/

    Energy& energy;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
