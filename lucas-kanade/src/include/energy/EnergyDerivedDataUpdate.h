#ifndef __LUCAS_KANADE_ENERGY_DERIVED_DATA_UPDATE_H__
#define __LUCAS_KANADE_ENERGY_DERIVED_DATA_UPDATE_H__

#include "energy/EnergyData.h"
#include "energy/EnergyDerivedData.h"
#include "energy/TransformationEnum.h"
#include "energy/ITKWrapper.h"

namespace lucasKanade{

  class EnergyDerivedDataUpdate {

  private:

    EnergyData& energyData;
    EnergyDerivedData& energyDerivedData;
    EnergySettings& energySettings;

    std::vector<arma::vec> transformedLocations;

  public:

    /*--------------------------------------------------------------------------*/

    EnergyDerivedDataUpdate(
                            EnergyData& energyData,
                            EnergyDerivedData& energyDerivedData,
                            EnergySettings& energySettings
                            ) :
      energyData(energyData),
      energyDerivedData(energyDerivedData),
      energySettings(energySettings) {
    }

    /*--------------------------------------------------------------------------*/

    void for_template() {

      this->energyDerivedData.templateImage.clear();

      for(int i = this->energyData.minX; i < this->energyData.maxX; ++i) {
        for(int j = this->energyData.minY; j < this->energyData.maxY; ++j) {
          for(int k = this->energyData.minZ; k < this->energyData.maxZ; ++k) {

            this->energyDerivedData.templateImage.push_back( this->energyData.templateImage.access().at_grid(i, j, k) );

          }
        }
      }

    }

    /*--------------------------------------------------------------------------*/

    // updates all data structures depending on the transformation parameters
    void for_transformation() {

      compute_transformation_matrix();

      compute_transformed_locations();

      compute_location_validity();

      compute_image_gradient_times_jacobian();

      compute_warped_image();

    }

    /*--------------------------------------------------------------------------*/

  private:

    void compute_transformation_matrix() {

      ITKWrapper::vnl_vector_to_transformation(this->energyDerivedData.transformation,
                                               this->energyData.center,
                                               this->energyDerivedData.transformationMatrix);

    }

    /*--------------------------------------------------------------------------*/

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

            this->energyDerivedData.imageGradientTimesJacobian.push_back(imageGradient.t() * J);

            ++index;

          }
        }
      }

    }

    /*--------------------------------------------------------------------------*/

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

  };
}
#endif
