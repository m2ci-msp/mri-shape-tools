#ifndef __ZERO_NORMALIZED_CROSS_CORRELATION_H__
#define __ZERO_NORMALIZED_CROSS_CORRELATION_H__

#include <armadillo>

#include "energy/EnergyData.h"
#include "energy/EnergyDerivedData.h"
#include "energy/TransformationEnum.h"
#include "energy/ITKWrapper.h"

// TODO:
// move candidates for refactoring outside of class

namespace lucasKanade{

  class ZNCC{

  private:

    // the mean color of the current deformed template
    double mean;

    // derivative of the mean
    arma::vec meanDerivative;

    EnergyDerivedData& energyDerivedData;

    std::vector<arma::vec> transformedLocations;

    EnergyData& energyData;

  public:

  private:

    /*--------------------------------------------------------------------------*/

    // compute transformed locations that are used later on to compute the warped
    // image
    // TODO: candidate for moving outside of the class
    void compute_transformed_locations() {

      this->transformedLocations.clear();

      for(double i = this->energyData.minX; i < this->energyData.maxX; ++i) {
        for(double j = this->energyData.minY; j < this->energyData.maxY; ++j) {
          for(double k = this->energyData.minZ; k < this->energyData.maxZ; ++k) {

            const arma::vec position({ i, j, k });
            this->transformedLocations.push_back(this->energyDerivedData.transformationMatrix.apply_matrix(position));

          }
        }
      }

    }

    /*--------------------------------------------------------------------------*/

    // mark transformed locations as invalid that are outside the image bounds
    // TODO: candidate for moving outside of class
    void compute_location_validity() {

      this->energyDerivedData.validLocation.clear();

      for(const arma::vec& position : this->transformedLocations) {

        const bool valid = ( position(0) < this->energyData.target.info().get_nx() &&
                             position(0) >= 0 &&
                             position(1) < this->energyData.target.info().get_ny() &&
                             position(1) >= 0 &&
                             position(2) < this->energyData.target.info().get_nz() &&
                             position(2) >= 0 );

        this->energyDerivedData.validLocation.push_back(valid);

      }

    }

    /*--------------------------------------------------------------------------*/

    // TODO: candidate for moving outside of class
    void compute_image_gradient_times_jacobian() {

      this->energyDerivedData.imageGradientTimesJacobian.clear();

      int index = 0;

      for(double i = this->energyData.minX; i < this->energyData.maxX; ++i) {
        for(double j = this->energyData.minY; j < this->energyData.maxY; ++j) {
          for(double k = this->energyData.minZ; k < this->energyData.maxZ; ++k) {

            if(this->energyDerivedData.validLocation.at(index) == false) {

              this->energyDerivedData.imageGradientTimesJacobian.push_back(arma::zeros(1, 1));
              ++index;

              continue;

            }

            const arma::mat J = compute_jacobian(i, j, k);

            const arma::vec& transformedLocation = this->transformedLocations.at(index);

            const arma::vec imageGradient = compute_image_gradient(transformedLocation);

            const arma::mat result = (imageGradient.t() * J);

            this->energyDerivedData.imageGradientTimesJacobian.push_back(result.row(0).t());

            ++index;

          }
        }
      }

    }

    /*--------------------------------------------------------------------------*/

    // compute the jacobian of the transformation matrix
    // TODO: candidate for moving outside of class
    arma::mat compute_jacobian(const double& x, const double& y, const double& z) const {

      const arma::vec p({ x, y, z });

      // compute derivatives
      const arma::vec dTx = this->energyDerivedData.transformationMatrix.apply_derivative_tx(p);
      const arma::vec dTy = this->energyDerivedData.transformationMatrix.apply_derivative_ty(p);
      const arma::vec dTz = this->energyDerivedData.transformationMatrix.apply_derivative_tz(p);

      const arma::vec dAlpha = this->energyDerivedData.transformationMatrix.apply_derivative_alpha(p);
      const arma::vec dBeta = this->energyDerivedData.transformationMatrix.apply_derivative_beta(p);
      const arma::vec dGamma = this->energyDerivedData.transformationMatrix.apply_derivative_gamma(p);


      // assemble jacobian matrix
      const arma::mat J({
          { dTx(0), dTy(0), dTz(0), dAlpha(0), dBeta(0), dGamma(0) },
            { dTx(1), dTy(1), dTz(1), dAlpha(1), dBeta(1), dGamma(1) },
              { dTx(2), dTy(2), dTz(2), dAlpha(2), dBeta(2), dGamma(2) }
        });

      return J;

    }

    /*--------------------------------------------------------------------------*/

    // TODO: candidate for moving outside of class
    arma::vec compute_image_gradient(const arma::vec& p) const {

      // interpolate values
      const double dx = this->energyData.targetX.interpolate().at_grid(p(0), p(1), p(2));
      const double dy = this->energyData.targetY.interpolate().at_grid(p(0), p(1), p(2));
      const double dz = this->energyData.targetZ.interpolate().at_grid(p(0), p(1), p(2));

      // assemble image gradient
      const arma::vec gradient( { dx, dy, dz });

      return gradient;

    }

    /*--------------------------------------------------------------------------*/

    // compute the warped image: this is the current deformed template
    // TODO: candidate for moving outside of class
    void compute_warped_image() {

      this->energyDerivedData.warpedImage.clear();

      int index = 0;

      for(int i = this->energyData.minX; i < this->energyData.maxX; ++i) {
        for(int j = this->energyData.minY; j < this->energyData.maxY; ++j) {
          for(int k = this->energyData.minZ; k < this->energyData.maxZ; ++k) {

            if(this->energyDerivedData.validLocation.at(index) == false) {

              this->energyDerivedData.warpedImage.push_back(0.);
              ++index;

              continue;

            }

            const arma::vec& transformedLocation = this->transformedLocations.at(index);

            const double warped = this->energyData.target.interpolate().at_grid(transformedLocation(0),
                                                                                transformedLocation(1),
                                                                                transformedLocation(2)
                                                                                );
            this->energyDerivedData.warpedImage.push_back(warped);

            ++index;

          }
        }
      }

    }

    /*--------------------------------------------------------------------------*/

    // use the current motion increment to deform the warped image
    void compute_modified_image() {

      this->modifiedImage.clear();

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          this->modifiedImage.push_back(0);
          continue;

        }

        const arma::vec& J = this->imageGradientTimesJacobian.at(i);

        const arma::vec increment( {
            this->energy.data().transformation[TX],
              this->energy.data().transformation[TY],
              this->energy.data().transformation[TZ],
              this->energy.data().transformation[ALPHA],
              this->energy.data().transformation[BETA],
              this->energy.data().transformation[GAMMA]
              }
          );

        this->modifiedImage.push_back(this->warpedImage.at(i) + arma::dot(J.t(), increment));

      }

    }

    /*--------------------------------------------------------------------------*/

    // compute the mean color in the modified image
    void compute_mean() {

      this->mean = 0;
      double count = 0;

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          continue;

        }

        mean += this->modifiedImage.at(i);
        ++count;

      }

      this->mean /= count;

    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of mean with respect to increment
    void compute_mean_derivative() {

      this->meanDerivative = arma::zeros(this->energyData.transformationAmount);
      double count = 0;

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          continue;

        }

        this->meanDerivative += this->imageGradientTimesJacobian.at(i);
        ++count;

      }

      this->meanDerivative /= count;

    }

    /*--------------------------------------------------------------------------*/

    // subtract the mean from each color of the modified image
    void compute_centered_values() {

      this->centeredValues.clear();

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          this->centeredValues.push_back(0);
          continue;

        }

        this->centeredValues.push_back(this->modifiedImage.at(i) - this->mean);

      }

    }

    /*--------------------------------------------------------------------------*/

    // compute the derivative of each centered value with respect to increment
    void compute_centered_values_derivatives() {

      this->centeredValuesDerivatives.clear();

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          this->centeredValuesDerivatives.push_back(arma::zeros(this->energyData.transformationAmount));
          continue;

        }

        this->centeredValuesDerivatives.push_back(

                                                  this->imageGradientTimesJacobian.at(i) - this->meanDerivative

                                                  );

      }


    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of squared version of the centered values
    void compute_squared_centered_values_derivatives() {

      this->squaredCenteredValuesDerivatives.clear();

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          this->squaredCenteredValuesDerivatives.push_back(arma::zeros(this->energyData.transformationAmount));
          continue;

        }

        this->squaredCenteredValuesDerivatives.push_back(
                                                         2. * this->centeredValuesDerivatives.at(i).t() *
                                                         this->centeredValues.at(i)
                                                         );

      }


    }

    /*--------------------------------------------------------------------------*/

    // compute the standard deviation of modified image
    void compute_standard_deviation() {

      this->standardDeviation = 0.;

      int count = 0;

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          continue;

        }

        this->standardDeviation += pow(this->centeredValues.at(i), 2.);
        ++count;

      }

      this->standardDeviation = sqrt( 1. / count * standardDeviationCenteredValues);

    }

    /*--------------------------------------------------------------------------*/

    // compute derivative of standard deviation with respect to increment
    void compute_standard_deviation_derivative() {

      this->standardDeviationDerivative = arma::zeros(this->energyData.transformationAmount);

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          continue;

        }

        this->standardDeviationDerivative += this->squaredCenteredValuesDerivatives.at(i);

      }

      this->standardDeviationDerivative *= 0.5 / this->standardDeviationCenteredValues;

    }

    /*--------------------------------------------------------------------------*/

    // normalize values by dividing by standard deviation
    void compute_normalized_values() {

      this->normalizedValues.clear();

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          this->normalizedValues.push_back(0);
          continue;

        }

        this->normalizedValues.push_back(this->centeredValues.at(i) / this->standardDeviation);

      }

    }

    /*--------------------------------------------------------------------------*/

    // take derivative of normalized values with respect to increment
    void compute_normalized_values_derivatives() {

      this->normalizedValuesDerivatives.clear();

      for(int i = 0; i < this->energyData.voxelAmount; ++i) {

        if(this->validLocation.at(i) == false) {

          this->normalizedValuesDerivatives.push_back(arma::zeros(this->energyData.transformationAmount));
          continue;

        }

        // quotient rule
        this->normalizedValuesDerivatives.push_back(

                                                    ( this->centeredValuesDerivatives.at(i) * this->standardDeviationCenteredValues - this->centeredValues.at(i) * this->standardDeviationDerivative ) /
                                                    pow(this->standardDeviation, 2 )

                                                    );

      }

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
