#ifndef __MATCH_TEMPLATE_DATA_TERM_H__
#define __MATCH_TEMPLATE_DATA_TERM_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "optimization/matchtemplate/Energy.h"
#include "optimization/matchtemplate/TransformationEnum.h"
#include "optimization/fitmodel/EnergyTerm.h"

namespace matchTemplate{

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

      // get weight for data term
      const double& baseFactor =
        this->energy.derived_data().weights.at("dataTerm");

      const int& transformationAmount =
        this->energy.data().transformationAmount;

      // get original vertices
      const std::vector<arma::vec>& vertices = this->energy.derived_data().source.get_vertices();

      const int vertexAmount = vertices.size();

      for( int i = 0; i < vertexAmount; ++i ) {

        // get vertex position and corresponding neighbor
        const arma::vec& vertex = vertices[i];
        const arma::vec& neighbor = this->energy.derived_data().neighbors[i];

        // use indicator if neighbor is present or not to construct factor
        const double factor = this->energy.derived_data().neighborIndicator[i] * baseFactor;

        const arma::vec difference = vertex - neighbor;

        // add energy
        energy += factor * arma::dot(difference, difference);

        // outer derivative for gradient computation
        const arma::vec outerDerivative = 2 * factor * difference;

        // get transformation to compute inner derivatives
        const Transformation& transformation =
          this->energy.derived_data().transformations.at(i);

        const int baseIndex = i * transformationAmount;

        gradient[baseIndex + TX] += arma::dot( outerDerivative, transformation.apply_derivative_tx(vertex) );
        gradient[baseIndex + TY] += arma::dot( outerDerivative, transformation.apply_derivative_ty(vertex) );
        gradient[baseIndex + TZ] += arma::dot( outerDerivative, transformation.apply_derivative_tz(vertex) );

        gradient[baseIndex + THETA] += arma::dot( outerDerivative, transformation.apply_derivative_theta(vertex) );
        gradient[baseIndex + AX]    += arma::dot( outerDerivative, transformation.apply_derivative_ax(vertex) );
        gradient[baseIndex + AY]    += arma::dot( outerDerivative, transformation.apply_derivative_ay(vertex) );
        gradient[baseIndex + AZ]    += arma::dot( outerDerivative, transformation.apply_derivative_az(vertex) );

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
