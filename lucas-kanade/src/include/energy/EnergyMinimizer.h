#ifndef __LUCAS_KANADE_ENERGY_MINIMIZER_H__
#define __LUCAS_KANADE_ENERGY_MINIMIZER_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/algo/vnl_lbfgsb.h>

#include "energy/Energy.h"
#include "energy/EnergyFunction.h"
#include "energy/ITKWrapper.h"
#include "energy/MinimizerSettings.h"

namespace lucasKanade{

  class EnergyMinimizer{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyMinimizer(
      Energy& energy,
      MinimizerSettings& settings
      ) :
      energy(energy),
      settings(settings) {

      setup_minimizer();

    }

    /*--------------------------------------------------------------------------*/

    ~EnergyMinimizer() {

      delete this->energyFunction;
      delete this->minimizer;

    }

    /*--------------------------------------------------------------------------*/

    void minimize() {

      // initialize data structures for current transformation
      this->energy.update().for_transformation();

      for(int i = 0; i < this->settings.iterationAmount; ++i) {

        perform_iteration();
        this->energy.state().energy = this->minimizer->get_end_error();

      }

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    void perform_iteration() {

      // find minimizer
      this->minimizer->minimize(this->energy.data().transformationIncrement);

      // increment transformation parameters and reset increment to 0
      for(int i = 0; i < this->energy.data().transformationAmount; ++i) {

        this->energy.data().transformation[i] += this->energy.data().transformationIncrement[i];
        this->energy.data().transformationIncrement[i] = 0;

      }

      // update data structures depending on current transformation parameters
      this->energy.update().for_transformation();

    }

    /*--------------------------------------------------------------------------*/

    void setup_minimizer() {

      const int& transformationAmount = this->energy.data().transformationAmount;

      vnl_vector<long> boundSelection(transformationAmount, 2);
      vnl_vector<double> lowerBounds(transformationAmount, 0.);
      vnl_vector<double> upperBounds(transformationAmount, 0.);

      // by default: allow every possible value
      for(int i = 0; i < transformationAmount; ++i) {
        lowerBounds[i] = - DBL_MAX;
        upperBounds[i] =   DBL_MAX;
      }

      this->energyFunction = new EnergyFunction(energy);
      this->minimizer = new vnl_lbfgsb(*this->energyFunction);

      this->minimizer->set_cost_function_convergence_factor(
        this->settings.convergenceFactor
        );

      this->minimizer->set_projected_gradient_tolerance(
        this->settings.projectedGradientTolerance
        );

      this->minimizer->set_max_function_evals(
        this->settings.maxFunctionEvals
        );

      this->minimizer->set_bound_selection(boundSelection);
      this->minimizer->set_lower_bound(lowerBounds);
      this->minimizer->set_upper_bound(upperBounds);

    }

    /*--------------------------------------------------------------------------*/

    Energy& energy;

    MinimizerSettings& settings;

    EnergyFunction* energyFunction;

    vnl_lbfgsb* minimizer;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
