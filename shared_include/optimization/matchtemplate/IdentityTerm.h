#ifndef __MATCH_TEMPLATE_IDENTITY_TERM_H__
#define __MATCH_TEMPLATE_IDENTITY_TERM_H__
#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "optimization/matchtemplate/Energy.h"
#include "optimization/matchtemplate/TransformationEnum.h"
#include "optimization/fitmodel/EnergyTerm.h"

namespace matchTemplate{

  /* class that represents the data term of the energy function */
  class IdentityTerm : public EnergyTerm {

  public:

    /*--------------------------------------------------------------------------*/

    IdentityTerm(Energy& energy) : energy(energy) {
    }

    /*--------------------------------------------------------------------------*/

    virtual void add_energy_and_gradient(
      double& energy,
      vnl_vector<double>& gradient) const {

      const int& transformationAmount =
        this->energy.data().transformationAmount;

      // weight for identity term
      const double factor = 1. / transformationAmount;

      for( int i = 0; i < transformationAmount; ++i ) {

        const double difference =
          this->energy.data().transformations[i] -
          this->energy.data().matchResult[i];

        // add energy
        energy += factor * pow(difference, 2);

        gradient[i] += 2 * difference;

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
