#ifndef __RIGID_ALIGNMENT_LANDMARK_TERM_H__
#define __RIGID_ALIGNMENT_LANDMARK_TERM_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "model/Model.h"
#include "optimization/fitmodel/EnergyTerm.h"
#include "optimization/rigidalignment/TransformationEnum.h"
#include "optimization/rigidalignment/Energy.h"

namespace rigidAlignment{

  /* class that represents the landmark term of the energy function */
  class LandmarkTerm : public EnergyTerm {

  public:

    /*--------------------------------------------------------------------------*/

    LandmarkTerm(Energy& energy) : energy(energy) {
    }

    /*--------------------------------------------------------------------------*/

    void add_energy_and_gradient(
      double& energy,
      vnl_vector<double>& gradient) const {

      const arma::vec difference =
        this->energy.derived_data().linearizedLandmarkSource -
        this->energy.derived_data().linearizedLandmarkTarget;

      // get weight for landmark term
      const double& factor =
        this->energy.derived_data().weights.at("landmarkTerm");

      // add energy
      energy += factor * arma::dot(difference, difference);

      // outer derivative
      const arma::vec outerDerivative = 2 * factor * difference;

      // inner derivatives
      std::vector<arma::vec> innerDerivatives = compute_inner_derivatives();

      // compute gradient
      for(int i = 0; i < this->energy.data().transformationAmount; ++i) {
        gradient[i] += arma::dot(innerDerivatives[i], outerDerivative);
      }

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    std::vector<arma::vec> compute_inner_derivatives() const {

      const Transformation& transformation =
        this->energy.data().transformationMatrix;

      const std::vector<arma::vec>& vertices =
        this->energy.data().source.get_vertices();

      const int vertexAmount = vertices.size();

      // initialize result vector
      std::vector<arma::vec> innerDerivatives;

      // initialize inner derivative components
      for(int i = 0; i < this->energy.data().transformationAmount; ++i) {
        innerDerivatives.push_back(arma::zeros(vertexAmount * 3));
      }

      // iterate over landmarks to compute inner derivatives
      for( const Landmark& mark: this->energy.data().landmarks ) {

        const int& i = mark.sourceIndex;

        const arma::vec& vertex = vertices[i];

        serialize(transformation.apply_derivative_tx(vertex), i, innerDerivatives.at(TX));
        serialize(transformation.apply_derivative_ty(vertex), i, innerDerivatives.at(TY));
        serialize(transformation.apply_derivative_tz(vertex), i, innerDerivatives.at(TZ));

//        serialize(transformation.apply_derivative_s(vertex), i, innerDerivatives.at(S));

        serialize(transformation.apply_derivative_theta(vertex), i, innerDerivatives.at(THETA));
        serialize(transformation.apply_derivative_ax(vertex), i, innerDerivatives.at(AX));
        serialize(transformation.apply_derivative_ay(vertex), i, innerDerivatives.at(AY));
        serialize(transformation.apply_derivative_az(vertex), i, innerDerivatives.at(AZ));

      }

      return innerDerivatives;

    } // end compute_inner_derivatives


    /*--------------------------------------------------------------------------*/

    // helper function for putting components of a vector into a target vector
    void serialize(
      const arma::vec& vector,
      const int& position,
      arma::vec& linearized) const {

      linearized[position * 3 + 0] = vector(0);
      linearized[position * 3 + 1] = vector(1);
      linearized[position * 3 + 2] = vector(2);

    }

    /*--------------------------------------------------------------------------*/

    Energy& energy;

    /*--------------------------------------------------------------------------*/

  };

}

#endif
