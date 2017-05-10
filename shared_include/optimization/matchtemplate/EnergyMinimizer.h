#ifndef __MATCH_TEMPLATE_ENERGY_MINIMIZER_H__
#define __MATCH_TEMPLATE_ENERGY_MINIMIZER_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>
#include <vnl/algo/vnl_lbfgsb.h>

#include "optimization/matchtemplate/Energy.h"
#include "optimization/matchtemplate/EnergyFunction.h"
#include "optimization/matchtemplate/DataTerm.h"
#include "optimization/matchtemplate/LandmarkTerm.h"
#include "optimization/matchtemplate/IdentityTerm.h"
#include "optimization/matchtemplate/SmoothnessTerm.h"
#include "optimization/matchtemplate/MinimizerSettings.h"

namespace matchTemplate{

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

      this->energyTerms.push_back(new SmoothnessTerm(energy));

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

      // initialize rotation axes to normals of source mesh
      setup_rotation_axes();

      // initialize data structures for current landmarks
      this->energy.update().for_landmarks();

      // initialize data structures for current transformation
      this->energy.update().for_transformations();

      // compute neighbors if fixed correspondences are used
      if(this->energy.neighbors().are_fixed() == true) {
        this->energy.neighbors().compute();
      }

      for(int i = 0; i < this->settings.iterationAmount; ++i) {
        perform_iteration(i);
        this->energy.state().energy = this->minimizer->get_end_error();
      }

      // try to remove possible landmark spikes
      remove_landmark_spikes();

      if( this->energy.settings().weights["postSmoothnessTerm"] > 0 ) {

        perform_post_smoothing();

      }

      // update normals for result
      this->energy.update().source_normals();

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    void remove_landmark_spikes() {

      const int& transformationAmount = this->energy.data().transformationAmount;

      for(const Landmark& mark: this->energy.data().landmarks) {

        const int& vertexId = mark.sourceIndex;

        // compute index of motion data for vertex
        const int vertexIndex = vertexId * transformationAmount;

        // get neighbors for vertex
        const std::set<int>& neighbors =
          this->energy.data().sourceNeighbors.get_neighbors(vertexId);

        // set transformations to zero
        for( int field = 0; field < transformationAmount; ++field ) {
          this->energy.data().transformations[vertexIndex + field] = 0;
        }

        // average over neighbors
        for( const int& neighborId : neighbors ) {

          const int neighborIndex = neighborId * transformationAmount;

          for( int field = 0; field < transformationAmount; ++field ) {
            this->energy.data().transformations[vertexIndex + field] +=
              this->energy.data().transformations[neighborIndex + field];
          }

        }

        for( int field = 0; field < transformationAmount; ++field ) {
          this->energy.data().transformations[vertexIndex + field] /= neighbors.size();
        }

      }

      this->energy.update().for_transformations();

    }

    /*--------------------------------------------------------------------------*/

    void perform_post_smoothing() {

      // copy result of template matching
      this->energy.data().matchResult = this->energy.data().transformations;

      // get rid of landmark term
      if( this->energyTerms.size() == 3 ) {
        delete this->energyTerms[2];
        this->energyTerms.pop_back();
      }

      // replace data term with identity term
      delete this->energyTerms[0];
      this->energyTerms[0] = new IdentityTerm(energy);

      // adjust smoothness term weight
      this->energy.derived_data().weights["smoothnessTerm"] =
        this->energy.settings().weights["postSmoothnessTerm"];

      // find minimizer
      this->minimizer->minimize(this->energy.data().transformations);

      // update data structures depending on weights
      this->energy.update().for_transformations();

    }

    /*--------------------------------------------------------------------------*/

    void perform_iteration(const int& i) {

      // modify smoothness term and landmark term weights for given iteration
      this->energy.update().for_iteration(i);

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
      this->minimizer->minimize(this->energy.data().transformations);

      // update data structures depending on weights
      this->energy.update().for_transformations();

    }

    /*--------------------------------------------------------------------------*/

    void setup_minimizer() {

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

    }

    /*--------------------------------------------------------------------------*/

    void setup_rotation_axes() {

      // compute normals for template
      NormalEstimation estimator(this->energy.data().source);
      this->energy.data().source.set_vertex_normals(estimator.compute());

      std::vector<arma::vec>& normals = this->energy.data().source.get_vertex_normals();

      const int& transformationAmount = this->energy.data().transformationAmount;

      vnl_vector<double>& transformations = this->energy.data().transformations;

      const int& vertexAmount = this->energy.data().vertexAmount;

      // initialize rotation axes to vertex normals
      for( int i = 0; i < vertexAmount; ++i) {

        const int baseIndex = i * transformationAmount;

        const arma::vec& normal = normals[i];

        transformations[baseIndex + AX] = normal(0);
        transformations[baseIndex + AY] = normal(1);
        transformations[baseIndex + AZ] = normal(2);


      }

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
