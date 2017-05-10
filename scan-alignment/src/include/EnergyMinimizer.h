#ifndef __ENERGY_MINIMIZER_H__
#define __ENERGY_MINIMIZER_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/algo/vnl_lbfgsb.h>

#include "Energy.h"
#include "EnergyFunction.h"
#include "DataTerm.h"
#include "MinimizerSettings.h"

class EnergyMinimizer{

public:

  /*--------------------------------------------------------------------------*/

  EnergyMinimizer(
    Energy& energy,
    MinimizerSettings& settings
    ) :
    energy(energy),
    settings(settings) {

    this->energyTerms.push_back(new DataTerm(energy));

    arma::vec lowerTranslation;
    arma::vec lowerRotation;

    arma::vec upperTranslation;
    arma::vec upperRotation;

    construct_box(
      lowerTranslation,
      lowerRotation,
      upperTranslation,
      upperRotation
      );

    setup_minimizer(
      lowerTranslation,
      lowerRotation,
      upperTranslation,
      upperRotation
      );

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

    for(int i = 0; i < this->settings.iterationAmount; ++i) {
      perform_iteration();
    }

    this->energy.info().energyValue =
      this->energyFunction->compute_only_current_energy();

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void perform_iteration() {

    // find minimizer
    this->minimizer->minimize(this->energy.data().transformation);

  }

  /*--------------------------------------------------------------------------*/

  void construct_box(
    arma::vec& lowerTranslation,
    arma::vec& lowerRotation,
    arma::vec& upperTranslation,
    arma::vec& upperRotation
    ) const {

    lowerTranslation = arma::vec(3);
    lowerTranslation.fill(-this->settings.translationLimit);

    lowerRotation = arma::vec(3);
    lowerRotation.fill(-this->settings.rotationLimit);

    upperTranslation = arma::vec(3);
    upperTranslation.fill(this->settings.translationLimit);

    upperRotation = arma::vec(3);
    upperRotation.fill(this->settings.rotationLimit);


  } // end construct_box_boundary_weights

  /*--------------------------------------------------------------------------*/

  void setup_minimizer(
    const arma::vec& lowerTranslation,
    const arma::vec& lowerRotation,
    const arma::vec& upperTranslation,
    const arma::vec& upperRotation
    ) {

    const int dimensionTranslation = 3;
    const int dimensionRotation = 3;

    vnl_vector<long> boundSelection(dimensionTranslation + dimensionRotation, 2);
    vnl_vector<double> lowerBounds(dimensionTranslation + dimensionRotation, 0.);
    vnl_vector<double> upperBounds(dimensionTranslation + dimensionRotation, 0.);

    for(int i = 0; i < dimensionTranslation; ++i) {
      lowerBounds[i] = lowerTranslation(i);
      upperBounds[i] = upperTranslation(i);
    }

    for(int i = 0; i < dimensionRotation; ++i) {
      lowerBounds[dimensionTranslation + i] = lowerRotation(i);
      upperBounds[dimensionTranslation + i] = upperRotation(i);
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

#endif
