#ifndef __MATCH_TEMPLATE_REGION_SMOOTHNESS_TERM_H__
#define __MATCH_TEMPLATE_REGION_SMOOTHNESS_TERM_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>
#include <cmath>

#include "optimization/matchtemplate/Energy.h"
#include "optimization/fitmodel/EnergyTerm.h"


namespace matchTemplate{

  class RegionSmoothnessTerm : public EnergyTerm {

  public:

    /*--------------------------------------------------------------------------*/

    RegionSmoothnessTerm(Energy& energy) : energy(energy) {
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
          this->energy.derived_data().smoothnessTermWeights[vertexId] / ( vertexAmount * neighborAmount );

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

        //fmod( this->energy.data().transformations[neighborTransformationId], 2 * M_PI );
      while( vertexAngle < 0 ) { vertexAngle += 2 * M_PI; }
      while( vertexAngle > 2 *  M_PI ) { vertexAngle -= 2 * M_PI; }

      while( neighborAngle < 0 ) { neighborAngle += 2 * M_PI; }
      while( neighborAngle > 2 *  M_PI ) { neighborAngle -= 2 * M_PI; }


//        fmod( this->energy.data().transformations[vertexTransformationId], 2 * M_PI );



      //  vertexAngle   = ( vertexAngle   < 0 ) ? vertexAngle   + 2 * M_PI : vertexAngle;
      //neighborAngle = ( neighborAngle < 0 ) ? neighborAngle + 2 * M_PI : neighborAngle;

      if( vertexAngle > neighborAngle ) {

        double difference = vertexAngle - neighborAngle;

        if( difference < 2 * M_PI - difference ) {

          // add energy
          energy += weight * pow(difference, 2);

          // add gradients
          gradient[vertexTransformationId]   +=   2 * weight * difference;
          gradient[neighborTransformationId] += - 2 * weight * difference;

        }
        else {

          difference = 2 * M_PI - difference;

          // add energy
          energy += weight * pow(difference, 2);

          // add gradients
          gradient[vertexTransformationId]   += - 2 * weight * difference;
          gradient[neighborTransformationId] +=   2 * weight * difference;

        }

      } else {

        double difference = neighborAngle - vertexAngle;

        if( difference < 2 * M_PI - difference ) {

          // add energy
          energy += weight * pow(difference, 2);

          // add gradients
          gradient[vertexTransformationId]   += - 2 * weight * difference;
          gradient[neighborTransformationId] +=   2 * weight * difference;

        }
        else {

          difference = 2 * M_PI - difference;

          // add energy
          energy += weight * pow(difference, 2);

          // add gradients
          gradient[vertexTransformationId]   +=   2 * weight * difference;
          gradient[neighborTransformationId] += - 2 * weight * difference;

        }


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
