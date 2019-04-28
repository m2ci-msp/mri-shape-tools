#ifndef __MATCH_TEMPLATE_SMOOTHNESS_TERM_H__
#define __MATCH_TEMPLATE_SMOOTHNESS_TERM_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>
#include <cmath>

#include "optimization/matchtemplate/Energy.h"
#include "optimization/fitmodel/EnergyTerm.h"


namespace matchTemplate{

  class SmoothnessTerm : public EnergyTerm {

  public:

    /*--------------------------------------------------------------------------*/

    SmoothnessTerm(Energy& energy) : energy(energy) {
    }

    /*--------------------------------------------------------------------------*/

    virtual void add_energy_and_gradient(
      double& energy,
      vnl_vector<double>& gradient) const {

      const int vertexAmount = this->energy.data().source.get_vertices().size();

      // iterate over vertices
      for(int vertexId = 0; vertexId < vertexAmount; ++vertexId) {

        // get neighbors for vertex
        const std::set<int>& neighbors =
          this->energy.data().sourceNeighbors.get_neighbors(vertexId);

        // and store amount of neighbors
        const double neighborAmount = neighbors.size();

        // compute weight of current smoothness term:
        // normalize by neighbor amount and complete vertex amount
        const double weight =
          this->energy.derived_data().weights["smoothnessTerm"] / ( vertexAmount * neighborAmount );

        // iterate over neighbors
        for( const int& neighborId : neighbors ) {

          compute_translational_part(vertexId, neighborId, weight, energy, gradient);
          compute_angular_part(vertexId, neighborId, weight, energy, gradient);

        } // end for neighborId

      } // end for vertexId

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    void compute_translational_part(
      /* INPUT */
      const int& vertexId,
      const int& neighborId,
      const double& weight,
      /* OUTPUT */
      double& energy,
      vnl_vector<double>& gradient
      ) const {

      // helper vector for iterating
      std::vector<int> translationFields( { TX, TY, TZ } );

      const int& transformationAmount = this->energy.data().transformationAmount;

      for( const int& field: translationFields ) {

        const int vertexTransformationId   = vertexId   * transformationAmount + field;
        const int neighborTransformationId = neighborId * transformationAmount + field;

        const double difference =
          this->energy.data().transformations[vertexTransformationId  ] -
          this->energy.data().transformations[neighborTransformationId];

        // add to energy
        energy += weight * pow(difference, 2);

        // add gradients
        gradient[vertexTransformationId]   +=   2 * weight * difference;
        gradient[neighborTransformationId] += - 2 * weight * difference;

      }

    }

    /*--------------------------------------------------------------------------*/

    void compute_angular_part(
      /* INPUT */
      const int& vertexId,
      const int& neighborId,
      const double& weight,
      /* OUTPUT */
      double& energy,
      vnl_vector<double>& gradient
      ) const {

      const int vertexTransformationId   = vertexId   * this->energy.data().transformationAmount + THETA;
      const int neighborTransformationId = neighborId * this->energy.data().transformationAmount + THETA;

      // shift to interval [0, 2 PI ]
      double vertexAngle =
        this->energy.data().transformations[vertexTransformationId];

      double neighborAngle =
        this->energy.data().transformations[neighborTransformationId];

      // map both angles to the interval [0, 2 pi]
      while( vertexAngle < 0 ) { vertexAngle += 2 * M_PI; }
      while( vertexAngle > 2 *  M_PI ) { vertexAngle -= 2 * M_PI; }

      while( neighborAngle < 0 ) { neighborAngle += 2 * M_PI; }
      while( neighborAngle > 2 *  M_PI ) { neighborAngle -= 2 * M_PI; }

      // evaluate theta_i - theta_j
      const double difference = vertexAngle - neighborAngle;

      // compute absolute value of difference
      const double absoluteValue = fabs(difference);

      // compute | theta_i - theta_j |^2 or ( 2 pi - |theta_i - theta_j| )^2
      // depending on which is smaller
      if( absoluteValue < 2 * M_PI - absoluteValue ) {

        // | theta_i - theta_j |^2 = ( theta_i - theta_j )^2

        // add energy
        energy += weight * pow(difference, 2);

        // add gradients
        gradient[vertexTransformationId]   +=   2 * weight * difference;
        gradient[neighborTransformationId] += - 2 * weight * difference;

      } else {

        // 2 pi - | theta_i - theta_j |
        const double smallestDifference = 2 * M_PI - absoluteValue;

        // add energy
        energy += weight * pow(smallestDifference, 2);

        // derivative of
        // - | theta_i - theta_j | with respect to theta_i :
        //
        // - ( theta_i - theta_j ) / | theta_i - theta_j |
        //
        // taking the derivative with respect to theta_j changes the sign
        // NOTE: absoluteValue will never be zero in this case
        const double derivative = - difference / absoluteValue;

        // complete derivative of ( 2 pi - | theta_i - theta_j | )^2 with respect to theta_i :
        //
        // 2 ( 2 pi - | theta_i - theta_j | ) ( - (theta_i - theta_j ) / | theta_i - theta_j |))

        gradient[vertexTransformationId]   +=   2 * weight * smallestDifference * derivative;
        gradient[neighborTransformationId] += - 2 * weight * smallestDifference * derivative;

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
