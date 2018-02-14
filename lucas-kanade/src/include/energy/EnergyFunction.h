#ifndef __LUCAS_KANADE_ENERGY_FUNCTION_H__
#define __LUCAS_KANADE_ENERGY_FUNCTION_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>

#include "energy/EnergyData.h"

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

      const int voxelAmount = this->energy.derived_data().templateImage.size();

      const std::vector<double>& warpedImage = this->energy.derived_data().warpedImage;

      const std::vector<bool>& validLocation = this->energy.derived_data().validLocation;

      const std::vector<double>& templateImage = this->energy.derived_data().templateImage;

      const std::vector<arma::vec>& imageGradientTimesJacobian = this->energy.derived_data().imageGradientTimesJacobian;

      for(int i = 0; i < voxelAmount; ++i ) {

        // skip if we don't have valid data for the current location
        if( validLocation.at(i) == false) {

          continue;

        }

        const arma::vec& J = imageGradientTimesJacobian.at(i);

        const arma::vec increment( {
            this->energy.data().transformation[TX],
              this->energy.data().transformation[TY],
              this->energy.data().transformation[TZ],
              this->energy.data().transformation[ALPHA],
              this->energy.data().transformation[BETA],
              this->energy.data().transformation[GAMMA]
              }
          );

        const double difference = warpedImage.at(i) - templateImage.at(i) + arma::dot(J.t(), increment);

        energy += pow(difference, 2);

        const arma::mat tmp = 2 * J.t() * difference;

        const arma::vec localGradient = tmp.row(0).t();

        for(int j = 0; j < transformationAmount; ++j) {

          gradient[j] += localGradient(j);

        }

      }

    }

    /*--------------------------------------------------------------------------*/

    Energy& energy;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
