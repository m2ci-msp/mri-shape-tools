#ifndef __RIGID_ALIGNMENT_ENERGY_MINIMIZER_H__
#define __RIGID_ALIGNMENT_ENERGY_MINIMIZER_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/algo/vnl_lbfgsb.h>

#include "optimization/rigidalignment/Energy.h"
#include "optimization/rigidalignment/EnergyFunction.h"
#include "optimization/rigidalignment/DataTerm.h"
#include "optimization/rigidalignment/LandmarkTerm.h"
#include "optimization/rigidalignment/ITKWrapper.h"
#include "optimization/rigidalignment/MinimizerSettings.h"

namespace rigidAlignment{

  class EnergyMinimizer{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyMinimizer(
      Energy& energy,
      MinimizerSettings& settings
      ) :
      energy(energy),
      settings(settings) {

      // create needed energy terms
      this->energyTerms.push_back(new DataTerm(energy));

      if(this->energy.data().landmarks.size() > 0) {
        this->energyTerms.push_back(new LandmarkTerm(energy));
      }

      setup_minimizer();

    }

    /*--------------------------------------------------------------------------*/

    ~EnergyMinimizer() {
      delete this->energyFunction;
      delete this->minimizer;
      for(EnergyTerm* energyTerm : this->energyTerms) {
        delete energyTerm;
      }
    }

    /*--------------------------------------------------------------------------*/

    void minimize() {

      // initialize data structures for current transformation
      this->energy.update().for_transformation();

      // initialize data structures for current landmarks
      this->energy.update().for_landmarks();

      // compute neighbors if fixed correspondences are used
      if(this->energy.neighbors().are_fixed() == true) {
        this->energy.neighbors().compute();
      }

      for(int i = 0; i < this->settings.iterationAmount; ++i) {
        perform_iteration();
        this->energy.state().energy = this->minimizer->get_end_error();
      }

      // update normals for result
      if( this->energy.derived_data().source.has_faces() == true ) {
          this->energy.update().source_normals();
      }

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    void perform_iteration() {

      // setup source normals if needed
      if( this->energy.neighbors().need_normals() ) {
        this->energy.update().source_normals();
      }

      // set up correspondences for current iteration if needed
      if( this->energy.neighbors().are_fixed() == false ) {
        this->energy.neighbors().compute();
      }

      // update data structures that depend on neighbors
      this->energy.update().for_neighbors();

      // find minimizer
      this->minimizer->minimize(this->energy.data().transformation);

      // set new weights
      ITKWrapper::vnl_vector_to_transformation(
        this->energy.data().transformation,
        this->energy.data().transformationMatrix,
        this->energy.data().source
        );

      // update data structures depending on weights
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

      // avoid decimation
      lowerBounds[SX] = 0.1;
      lowerBounds[SY] = 0.1;
      lowerBounds[SZ] = 0.1;

      if( this->settings.noRotation == true ) {

        // leave angles at 0
        lowerBounds[ALPHA] = 0;
        upperBounds[ALPHA] = 0;

        lowerBounds[BETA] = 0;
        upperBounds[BETA] = 0;

        lowerBounds[GAMMA] = 0;
        upperBounds[GAMMA] = 0;

      }

      if( this->settings.noTranslation == true ) {

        // leave translation at 0
        lowerBounds[TX] = 0;
        upperBounds[TX] = 0;
        lowerBounds[TY] = 0;
        upperBounds[TY] = 0;
        lowerBounds[TZ] = 0;
        upperBounds[TZ] = 0;

      }

      if( this->settings.noScaling == true ) {

        // leave scaling at 1
        lowerBounds[SX] = 1;
        upperBounds[SX] = 1;

        lowerBounds[SY] = 1;
        upperBounds[SY] = 1;

        lowerBounds[SZ] = 1;
        upperBounds[SZ] = 1;
      }


      this->energyFunction = new EnergyFunction(energy, this->energyTerms);
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

    std::vector<EnergyTerm*> energyTerms;

    MinimizerSettings& settings;

    EnergyFunction* energyFunction;

    vnl_lbfgsb* minimizer;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
