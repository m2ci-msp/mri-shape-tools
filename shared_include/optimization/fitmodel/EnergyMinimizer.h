#ifndef __FIT_MODEL_ENERGY_MINIMIZER_H__
#define __FIT_MODEL_ENERGY_MINIMIZER_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/algo/vnl_lbfgsb.h>

#include "optimization/fitmodel/Energy.h"
#include "optimization/fitmodel/EnergyFunction.h"
#include "optimization/fitmodel/DataTerm.h"
#include "optimization/fitmodel/LandmarkTerm.h"
#include "optimization/fitmodel/SmoothnessTerm.h"
#include "optimization/fitmodel/MeanBiasTerm.h"
#include "optimization/fitmodel/ITKWrapper.h"
#include "optimization/fitmodel/MinimizerSettings.h"

namespace fitModel{

  class EnergyMinimizer{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyMinimizer(
      Energy& energy,
      MinimizerSettings& settings
      ) :
      energy(energy),
      settings(settings) {

      const Model& model = this->energy.data().model;

      const int dimensionSpeaker = model.data().get_speaker_mode_dimension();
      const int dimensionPhoneme = model.data().get_phoneme_mode_dimension();

      const int numWeights = dimensionSpeaker + dimensionPhoneme;

      this->weightAmount = numWeights;

      arma::vec lowerSpeaker;
      arma::vec lowerPhoneme;
      arma::vec upperSpeaker;
      arma::vec upperPhoneme;

      construct_box_boundary_weights(
        lowerSpeaker, lowerPhoneme, upperSpeaker, upperPhoneme);

      // check for PCA model
      if( (dimensionSpeaker == 1 || dimensionPhoneme == 1) && settings.noPCA == false ) {
        adapt_box_to_pca(
          lowerSpeaker, lowerPhoneme, upperSpeaker, upperPhoneme);
      }

      // create needed energy terms
      this->energyTerms.push_back(new DataTerm(energy));

      if(this->energy.data().landmarks.size() > 0) {
        this->energyTerms.push_back(new LandmarkTerm(energy));
      }

      if(this->energy.settings().weights.at("speakerSmoothnessTerm") > 0 ||
         this->energy.settings().weights.at("phonemeSmoothnessTerm") > 0) {
        this->energyTerms.push_back(new SmoothnessTerm(energy));
      }

      if(this->energy.settings().weights.at("meanBiasTerm") > 0){
        this->energyTerms.push_back(new MeanBiasTerm(energy));
      }

      setup_minimizer(
        lowerSpeaker, lowerPhoneme, upperSpeaker, upperPhoneme);

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

      // initialize data structures for current weights
      this->energy.update().for_weights();

      // initialize data structures for current landmarks
      this->energy.update().for_landmarks();

      // compute neighbors if fixed correspondences are used
      if(this->energy.neighbors().are_fixed() == true) {
        this->energy.neighbors().compute();
      }

      // save old weights
      this->energy.data().oldSpeakerWeights =
        this->energy.data().speakerWeights;
      this->energy.data().oldPhonemeWeights =
        this->energy.data().phonemeWeights;

      // start on the mean -> avoid getting stuck at the boundary of the prior box
      this->energy.data().speakerWeights = this->energy.data().model.data().get_speaker_mean_weights();
      this->energy.data().phonemeWeights = this->energy.data().model.data().get_phoneme_mean_weights();

      for(int i = 0; i < this->settings.iterationAmount; ++i) {
        perform_iteration();
        this->energy.state().energy = this->minimizer->get_end_error();
      }

      // update normals for result
      if( this->energy.data().model.data().is_truncated() == false ) {
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

      // use current weights as initialization
      vnl_vector<double> x(this->weightAmount, 0.);

      ITKWrapper::weights_to_vnl_vector(
        this->energy.data().speakerWeights,
        this->energy.data().phonemeWeights,
        x
        );

      // find minimizer
      this->minimizer->minimize(x);

      // set new weights
      ITKWrapper::vnl_vector_to_weights(
        x,
        this->energy.data().speakerWeights,
        this->energy.data().phonemeWeights
        );

      // update data structures depending on weights
      this->energy.update().for_weights();

    }

    /*--------------------------------------------------------------------------*/

    void construct_box_boundary_weights(
      arma::vec& lowerSpeaker,
      arma::vec& lowerPhoneme,
      arma::vec& upperSpeaker,
      arma::vec& upperPhoneme
      ) const {

      const Model& model = this->energy.data().model;

      const int dimensionSpeaker = model.data().get_speaker_mode_dimension();
      const int dimensionPhoneme = model.data().get_phoneme_mode_dimension();

      lowerSpeaker = arma::vec(dimensionSpeaker);
      lowerSpeaker.fill(-this->settings.priorSize);

      lowerPhoneme = arma::vec(dimensionPhoneme);
      lowerPhoneme.fill(-this->settings.priorSize);

      upperSpeaker = arma::vec(dimensionSpeaker);
      upperSpeaker.fill(this->settings.priorSize);

      upperPhoneme = arma::vec(dimensionPhoneme);
      upperPhoneme.fill(this->settings.priorSize);

      model.convert().to_weights(lowerSpeaker, lowerPhoneme);
      model.convert().to_weights(upperSpeaker, upperPhoneme);

    } // end construct_box_boundary_weights

    /*--------------------------------------------------------------------------*/

    // adapt box to pca model -> do not optimize mode with one entry
    void adapt_box_to_pca(
      arma::vec& lowerSpeaker,
      arma::vec& lowerPhoneme,
      arma::vec& upperSpeaker,
      arma::vec& upperPhoneme
      ) const {

      const Model& model = this->energy.data().model;

      if( lowerSpeaker.n_elem == 1) {
        // do not optimize speaker component -> stays on the mean
        lowerSpeaker(0) = model.data().get_speaker_mean_weights()(0);
        upperSpeaker(0) = model.data().get_speaker_mean_weights()(0);
      }

      if( lowerPhoneme.n_elem == 1) {
        // do not optimize phoneme component -> stays on the mean
        lowerPhoneme(0) = model.data().get_phoneme_mean_weights()(0);
        upperPhoneme(0) = model.data().get_phoneme_mean_weights()(0);
      }

    } // end adapt_box_to_pca

    /*--------------------------------------------------------------------------*/

    void setup_minimizer(
      const arma::vec& lowerSpeaker,
      const arma::vec& lowerPhoneme,
      const arma::vec& upperSpeaker,
      const arma::vec& upperPhoneme
      ) {

      const int dimensionSpeaker = lowerSpeaker.size();
      const int dimensionPhoneme = lowerPhoneme.size();

      vnl_vector<long> boundSelection(this->weightAmount, 2);
      vnl_vector<double> lowerBounds(this->weightAmount, 0.);
      vnl_vector<double> upperBounds(this->weightAmount, 0.);

      for(int i = 0; i < dimensionSpeaker; ++i) {
        lowerBounds[i] = lowerSpeaker(i);
        upperBounds[i] = upperSpeaker(i);
      }

      for(int i = 0; i < dimensionPhoneme; ++i) {
        lowerBounds[dimensionSpeaker + i] = lowerPhoneme(i);
        upperBounds[dimensionSpeaker + i] = upperPhoneme(i);
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

    int weightAmount;


    /*--------------------------------------------------------------------------*/

  };

}
#endif
