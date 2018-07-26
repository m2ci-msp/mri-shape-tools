#ifndef __RIGID_ALIGNMENT_DATA_TERM_H__
#define __RIGID_ALIGNMENT_DATA_TERM_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "optimization/rigidalignment/Energy.h"
#include "optimization/rigidalignment/TransformationEnum.h"
#include "optimization/fitmodel/EnergyTerm.h"

namespace rigidAlignment{

  /* class that represents the data term of the energy function */
  class DataTerm : public EnergyTerm {

  public:

    /*--------------------------------------------------------------------------*/

    DataTerm(Energy& energy) : energy(energy) {
    }

    /*--------------------------------------------------------------------------*/

    virtual void add_energy_and_gradient(
      double& energy,
      vnl_vector<double>& gradient) const {


      // compute difference
      const arma::vec difference =
        this->energy.derived_data().linearizedSource -
        this->energy.derived_data().linearizedTarget;

      // get weight for data term
      const double& factor =
        this->energy.derived_data().weights.at("dataTerm");

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

      // iterate over vertices to compute inner derivatives
      for( int i = 0; i < vertexAmount; ++i) {

        const arma::vec& vertex = vertices[i];

        serialize(transformation.apply_derivative_tx(vertex), i, innerDerivatives.at(TX));
        serialize(transformation.apply_derivative_ty(vertex), i, innerDerivatives.at(TY));
        serialize(transformation.apply_derivative_tz(vertex), i, innerDerivatives.at(TZ));

        serialize(transformation.apply_derivative_sx(vertex), i, innerDerivatives.at(SX));
        serialize(transformation.apply_derivative_sy(vertex), i, innerDerivatives.at(SY));
        serialize(transformation.apply_derivative_sz(vertex), i, innerDerivatives.at(SZ));

        serialize(transformation.apply_derivative_alpha(vertex), i, innerDerivatives.at(ALPHA));
        serialize(transformation.apply_derivative_beta(vertex), i, innerDerivatives.at(BETA));
        serialize(transformation.apply_derivative_gamma(vertex), i, innerDerivatives.at(GAMMA));

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
