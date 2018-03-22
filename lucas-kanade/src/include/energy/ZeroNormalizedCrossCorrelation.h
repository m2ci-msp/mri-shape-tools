#ifndef __ZERO_NORMALIZED_CROSS_CORRELATION_H__
#define __ZERO_NORMALIZED_CROSS_CORRELATION_H__

#include <vector>

#include <armadillo>

#include "DeformedTemplate.h"
#include "IncrementallyDeformedTemplate.h"

namespace lucasKanade{

  class ZeroNormalizedCrossCorrelation{

  private:

    // derived data
    int transformationAmount;
    int voxelAmount;

    // input
    const std::vector<double>& originalNormalizedValues;
    const DeformedTemplate& deformedTemplate;
    const IncrementallyDeformedTemplate& incrementallyDeformedTemplate;

    // immediate results
    // the mean color of the current deformed template
    double mean;

    // derivative of the mean
    arma::vec meanDerivative;

    // values centered to the mean
    std::vector<double> centeredValues;

    std::vector<arma::vec> centeredValuesDerivatives;

    std::vector<arma::vec> squaredCenteredValuesDerivatives;

    double standardDeviation;
    arma::vec standardDeviationDerivative;
    std::vector<double> normalizedValues;
    std::vector<arma::vec> normalizedValuesDerivatives;

    // output
    double correlation;

    arma::vec correlationDerivative;

  public:

    ZeroNormalizedCrossCorrelation(
                                   const std::vector<double>& originalNormalizedValues,
                                   const DeformedTemplate& deformedTemplate,
                                   const IncrementallyDeformedTemplate& incrementallyDeformedTemplate
                                   ) :

      originalNormalizedValues(originalNormalizedValues),
      deformedTemplate(deformedTemplate),
      incrementallyDeformedTemplate(incrementallyDeformedTemplate) {

      this->transformationAmount = 6;
      this->voxelAmount = this->deformedTemplate.get_location_valid().size();

      compute_mean();
      compute_mean_derivative();

      compute_centered_values();
      compute_centered_values_derivatives();

      compute_standard_deviation();
      compute_standard_deviation_derivative();

      compute_normalized_values();
      compute_normalized_values_derivatives();

      compute_correlation();
      compute_correlation_derivative();

    }

    /*--------------------------------------------------------------------------*/

    const double& get_correlation() const {

      return this->correlation;

    }

    /*--------------------------------------------------------------------------*/

    const arma::vec& get_correlation_derivative() const {

      return this->correlationDerivative;

    }

    /*--------------------------------------------------------------------------*/


  private:

    /*--------------------------------------------------------------------------*/

    // compute the mean color in the modified image
    void compute_mean() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      const std::vector<double>& incrementallyDeformedTemplateValues =
        this->incrementallyDeformedTemplate.get_incrementally_deformed_template();

      this->mean = 0;
      double count = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          continue;

        }

        mean += incrementallyDeformedTemplateValues[i];
        ++count;

      }

      this->mean /= count;

    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of mean with respect to increment
    void compute_mean_derivative() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      const std::vector<arma::mat>& imageGradientTimesJacobian =
        this->incrementallyDeformedTemplate.get_image_gradient_times_jacobian();

      this->meanDerivative = arma::zeros(this->transformationAmount);

      double count = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          continue;

        }

        this->meanDerivative += imageGradientTimesJacobian[i];
        ++count;

      }

      this->meanDerivative /= count;

    }

    /*--------------------------------------------------------------------------*/

    // subtract the mean from each color of the modified image
    void compute_centered_values() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      const std::vector<double>& incrementallyDeformedTemplateValues =
        this->incrementallyDeformedTemplate.get_incrementally_deformed_template();

      this->centeredValues.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          this->centeredValues.push_back(0);
          continue;

        }

        this->centeredValues.push_back(incrementallyDeformedTemplateValues[i] - this->mean);

      }

    }

    /*--------------------------------------------------------------------------*/

    // compute the derivative of each centered value with respect to increment
    void compute_centered_values_derivatives() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      const std::vector<arma::mat>& imageGradientTimesJacobian =
        this->incrementallyDeformedTemplate.get_image_gradient_times_jacobian();

      this->centeredValuesDerivatives.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          this->centeredValuesDerivatives.push_back(arma::zeros(this->transformationAmount));
          continue;

        }

        this->centeredValuesDerivatives.push_back(

                                                  imageGradientTimesJacobian[i] - this->meanDerivative

                                                  );

      }

    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of squared version of the centered values
    void compute_squared_centered_values_derivatives() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      this->squaredCenteredValuesDerivatives.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          this->squaredCenteredValuesDerivatives.push_back(arma::zeros(this->transformationAmount));
          continue;

        }

        this->squaredCenteredValuesDerivatives.push_back(
                                                         2. * this->centeredValuesDerivatives[i].t() *
                                                         this->centeredValues[i]
                                                         );

      }


    }

    /*--------------------------------------------------------------------------*/

    // compute the standard deviation of modified image
    void compute_standard_deviation() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      this->standardDeviation = 0.;

      int count = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          continue;

        }

        this->standardDeviation += pow(this->centeredValues[i], 2.);
        ++count;

      }

      this->standardDeviation = sqrt( 1. / count * standardDeviation);

    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of standard deviation with respect to increment
    void compute_standard_deviation_derivative() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      this->standardDeviationDerivative = arma::zeros(this->transformationAmount);

      int count = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          continue;

        }

        this->standardDeviationDerivative += this->squaredCenteredValuesDerivatives[i];
        ++count;

      }

      this->standardDeviationDerivative /= count;
      this->standardDeviationDerivative *= 0.5 / this->standardDeviation;

    }

    /*--------------------------------------------------------------------------*/

    // normalize values by dividing by standard deviation
    void compute_normalized_values() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      this->normalizedValues.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          this->normalizedValues.push_back(0);
          continue;

        }

        this->normalizedValues.push_back(this->centeredValues[i] / this->standardDeviation);

      }

    }

    /*--------------------------------------------------------------------------*/

    // take derivative of normalized values with respect to increment
    void compute_normalized_values_derivatives() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      this->normalizedValuesDerivatives.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          this->normalizedValuesDerivatives.push_back(arma::zeros(this->transformationAmount));
          continue;

        }

        // quotient rule
        this->normalizedValuesDerivatives.push_back(

                                                    ( this->centeredValuesDerivatives[i] * this->standardDeviation- this->centeredValues[i] * this->standardDeviationDerivative ) /
                                                    pow(this->standardDeviation, 2 )

                                                    );

      }

    }

    /*--------------------------------------------------------------------------*/

    void compute_correlation() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      this->correlation = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          continue;

        }

        this->correlation += this->originalNormalizedValues[i] * this->normalizedValues[i];

      }

    }

    /*--------------------------------------------------------------------------*/

    void compute_correlation_derivative() {

      const std::vector<bool>& locationValid = this->deformedTemplate.get_location_valid();

      this->correlationDerivative = arma::zeros(this->transformationAmount);

      for(int i = 0; i < this->voxelAmount; ++i) {

        if(locationValid[i] == false) {

          continue;

        }

        this->correlationDerivative += this->originalNormalizedValues[i] * this->normalizedValuesDerivatives[i];

      }

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
