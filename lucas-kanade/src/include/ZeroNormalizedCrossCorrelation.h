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

    std::vector<arma::mat> centeredValuesDerivatives;

    std::vector<arma::mat> squaredCenteredValuesDerivatives;

    double norm;
    arma::mat normDerivative;
    std::vector<double> normalizedValues;
    std::vector<arma::mat> normalizedValuesDerivatives;

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
      this->voxelAmount = this->deformedTemplate.get_deformed_template().size();

      compute_mean();
      compute_mean_derivative();

      compute_centered_values();
      compute_centered_values_derivatives();
      compute_squared_centered_values_derivatives();

      compute_norm();
      compute_norm_derivative();

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

      const std::vector<double>& incrementallyDeformedTemplateValues =
        this->incrementallyDeformedTemplate.get_incrementally_deformed_template();

      this->mean = 0;
      double count = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        mean += incrementallyDeformedTemplateValues[i];
        ++count;

      }

      this->mean /= (count > 0) ? count : 1;

    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of mean with respect to increment
    void compute_mean_derivative() {

      const std::vector<arma::mat>& imageGradientTimesJacobian =
        this->incrementallyDeformedTemplate.get_image_gradient_times_jacobian();

      this->meanDerivative = arma::zeros(this->transformationAmount);

      double count = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->meanDerivative += imageGradientTimesJacobian[i];
        ++count;

      }

      this->meanDerivative /= ( count > 0)? count: 1;

    }

    /*--------------------------------------------------------------------------*/

    // subtract the mean from each color of the modified image
    void compute_centered_values() {

      const std::vector<double>& incrementallyDeformedTemplateValues =
        this->incrementallyDeformedTemplate.get_incrementally_deformed_template();

      this->centeredValues.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->centeredValues.push_back(incrementallyDeformedTemplateValues[i] - this->mean);

      }

    }

    /*--------------------------------------------------------------------------*/

    // compute the derivative of each centered value with respect to increment
    void compute_centered_values_derivatives() {

      const std::vector<arma::mat>& imageGradientTimesJacobian =
        this->incrementallyDeformedTemplate.get_image_gradient_times_jacobian();

      this->centeredValuesDerivatives.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->centeredValuesDerivatives.push_back(

                                                  (imageGradientTimesJacobian[i] - this->meanDerivative).t()

                                                  );

      }

    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of squared version of the centered values
    void compute_squared_centered_values_derivatives() {

      this->squaredCenteredValuesDerivatives.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->squaredCenteredValuesDerivatives.push_back(
                                                         2. * this->centeredValuesDerivatives[i] *
                                                         this->centeredValues[i]
                                                         );

      }


    }

    /*--------------------------------------------------------------------------*/

    // compute the norm of the modified image
    void compute_norm() {

      this->norm = 0.;

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->norm += pow(this->centeredValues[i], 2.);

      }

      this->norm = sqrt(this->norm);

    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of standard deviation with respect to increment
    void compute_norm_derivative() {

      this->normDerivative = arma::zeros(this->transformationAmount).t();

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->normDerivative += this->squaredCenteredValuesDerivatives[i];

      }

      this->normDerivative *= 0.5 / this->norm;

    }

    /*--------------------------------------------------------------------------*/

    // normalize values by dividing by standard deviation
    void compute_normalized_values() {

      this->normalizedValues.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->normalizedValues.push_back(this->centeredValues[i] / this->norm);

      }

    }

    /*--------------------------------------------------------------------------*/

    // take derivative of normalized values with respect to increment
    void compute_normalized_values_derivatives() {

      this->normalizedValuesDerivatives.clear();

      for(int i = 0; i < this->voxelAmount; ++i) {

        // quotient rule
        this->normalizedValuesDerivatives.push_back(

                                                    ( this->centeredValuesDerivatives[i] * this->norm - this->centeredValues[i] * this->normDerivative ) /
                                                    pow(this->norm, 2 )

                                                    );


      }

    }

    /*--------------------------------------------------------------------------*/

    void compute_correlation() {

      this->correlation = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->correlation += this->originalNormalizedValues[i] * this->normalizedValues[i];

      }

    }

    /*--------------------------------------------------------------------------*/

    void compute_correlation_derivative() {

      arma::mat derivative = arma::zeros(this->transformationAmount).t();

      // this->correlationDerivative = arma::zeros(this->transformationAmount).t();

      for(int i = 0; i < this->voxelAmount; ++i) {

        derivative += this->originalNormalizedValues[i] * this->normalizedValuesDerivatives[i];

      }

      this->correlationDerivative = derivative.t();

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
