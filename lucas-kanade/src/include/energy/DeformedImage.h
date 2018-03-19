#ifndef __DEFORMED_IMAGE_H__
#define __DEFORMED_IMAGE_H__

#include <vector>
#include <armadillo>

#include "image/Image.h"
#include "matrix/Transformation.h"

namespace lucasKanade{

  class DeformedImage{

  private:

    /*--------------------------------------------------------------------------*/

    // these are the undeformed voxel coordinates of the subimage that is used as template
    const std::vector<arma::vec> originalLocations;

    const Image& originalImage;

    // derivatives of the original image
    Image imageX;
    Image imageY;
    Image imageZ;

    // values depending on the current transformation matrix
    std::vector<arma::vec> transformedLocations;

    std::vector<bool> locationValid;

    std::vector<arma::mat> imageGradientTimesJacobian;

    std::vector<double> warpedImage;

    // flag indicating if warped image was computed
    bool warpedComputed = false;

    // flag indicating if increment warped image was computed
    bool incrementWarpedComputed = false;

    // transformation matrix
    Transformation transformationMatrix;

    // transformation increment in linearization of transformation
    arma::vec transformationIncrement;

    /*--------------------------------------------------------------------------*/

  public:

    DeformedImage(
                  const Image& originalImage,
                  const std::vector<arma::vec>& originalLocations
                  ) : originalImage(originalImage), originalLocations(originalLocations) {

      compute_image_derivatives();

    }

    /*--------------------------------------------------------------------------*/

    void update_warped_image(const Transformation& transformation) {

      this->transformationMatrix = transformation;

      compute_transformed_locations();

      compute_location_validity();

      compute_image_gradient_times_jacobian();

      compute_warped_image();

      this->warpedComputed = true;

    }

    /*--------------------------------------------------------------------------*/

    void update_increment_warped_image(const arma::vec& increment) {

      if( this->warpedComputed == false ) {

        throw std::logic_error("Error: Cannot compute increment_warped_image without warped image.");

      }

      this->transformationIncrement = increment;

      compute_increment_warped_image();

      this->incrementWarpedComputed = true;

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    void compute_image_derivatives() {

      this->originalImage.boundary().change(1, 1, 1);
      this->originalImage.mirror().all();

      this->imageX = this->originalImage;
      this->imageY = this->originalImage;
      this->imageZ = this->originalImage;

      for(int i = 0; i < this->originalImage.info().get_nx(); ++i) {
        for(int j = 0; j < this->originalImage.info().get_ny(); ++j) {
          for(int k = 0; k < this->originalImage.info().get_nz(); ++k) {

            this->imageX.access().at_grid(i, j, k) =
              this->originalImage.calculus().dx(i, j, k);

            this->imageY.access().at_grid(i, j, k) =
              this->originalImage.calculus().dy(i, j, k);

            this->imageZ.access().at_grid(i, j, k) =
              this->originalImage.calculus().dz(i, j, k);

          }

        }

      }

      this->imageX.mirror().all();
      this->imageY.mirror().all();
      this->imageZ.mirror().all();

    }

    /*--------------------------------------------------------------------------*/

    // compute transformed locations that are used later on to compute the warped
    // image
    void compute_transformed_locations() {

      this->transformedLocations.clear();

      for(const arma::vec& location: this->originalLocations) {

        this->transformedLocations.push_back(this->transformationMatrix.apply_matrix(location));

      }

    }

    /*--------------------------------------------------------------------------*/

    // mark transformed locations as invalid that are outside the image bounds
    void compute_location_validity() {

      this->locationValid.clear();

      for(const arma::vec& position : this->transformedLocations) {

        const bool valid = ( position(0) < this->originalImage.info().get_nx() &&
                             position(0) >= 0 &&
                             position(1) < this->originalImage.info().get_ny() &&
                             position(1) >= 0 &&
                             position(2) < this->originalImage.info().get_nz() &&
                             position(2) >= 0 );

        this->locationValid.push_back(valid);

      }

    }

    /*--------------------------------------------------------------------------*/

    // compute the jacobian of the transformation matrix
    arma::mat compute_jacobian(const double& x, const double& y, const double& z) const {

      const arma::vec p({ x, y, z });

      // compute derivatives
      const arma::vec dTx = this->transformationMatrix.apply_derivative_tx(p);
      const arma::vec dTy = this->transformationMatrix.apply_derivative_ty(p);
      const arma::vec dTz = this->transformationMatrix.apply_derivative_tz(p);

      const arma::vec dAlpha = this->transformationMatrix.apply_derivative_alpha(p);
      const arma::vec dBeta = this->transformationMatrix.apply_derivative_beta(p);
      const arma::vec dGamma = this->transformationMatrix.apply_derivative_gamma(p);

      // assemble jacobian matrix
      const arma::mat J({
          { dTx(0), dTy(0), dTz(0), dAlpha(0), dBeta(0), dGamma(0) },
            { dTx(1), dTy(1), dTz(1), dAlpha(1), dBeta(1), dGamma(1) },
              { dTx(2), dTy(2), dTz(2), dAlpha(2), dBeta(2), dGamma(2) }
        });

      return J;

    }

    /*--------------------------------------------------------------------------*/

    void compute_image_gradient_times_jacobian() {

      this->imageGradientTimesJacobian.clear();

      for(size_t i = 0; i < this->originalLocations.size(); ++i) {

        if(this->locationValid[i] == false) {

          // insert dummy value
          this->imageGradientTimesJacobian.push_back(arma::zeros(1, 1));

          continue;

        }

        const arma::vec& location = this->originalLocations[i];

        const arma::mat J = compute_jacobian(location(0), location(1), location(2));

        const arma::vec& transformedLocation = this->transformedLocations[i];

        const arma::vec imageGradient = compute_image_gradient(transformedLocation);

        const arma::mat result = (imageGradient.t() * J);

        this->imageGradientTimesJacobian.push_back(result.row(0).t());

      } // end for i

    }

    /*--------------------------------------------------------------------------*/

    arma::vec compute_image_gradient(const arma::vec& p) const {

      // interpolate values
      const double dx = this->imageX.interpolate().at_grid(p(0), p(1), p(2));
      const double dy = this->imageY.interpolate().at_grid(p(0), p(1), p(2));
      const double dz = this->imageZ.interpolate().at_grid(p(0), p(1), p(2));

      // assemble image gradient
      const arma::vec gradient( { dx, dy, dz });

      return gradient;

    }

    /*--------------------------------------------------------------------------*/

    // compute the warped image: this is the current deformed template
    void compute_warped_image() {

      this->warpedImage.clear();

      for(size_t i = 0; i < this->transformedLocations.size(); ++i) {

        if(this->locationValid[i] == false) {

          this->warpedImage.push_back(0.);

          continue;

        }

        const arma::vec& transformedLocation = this->transformedLocations[i];

        const double warped = this->originalImage.interpolate().at_grid(transformedLocation(0),
                                                                        transformedLocation(1),
                                                                        transformedLocation(2)
                                                                        );
        this->warpedImage.push_back(warped);

      } // end for i

    }

    /*--------------------------------------------------------------------------*/

    // use the current transformation increment to deform the warped image
    void compute_increment_warped_image() {

      this->incrementWarpedImage.clear();

      for(size_t i = 0; i < this->warpedImage.size(); ++i) {

        if(this->locationValid[i] == false) {

          this->incrementWarpedImage.push_back(0);
          continue;

        }

        const arma::vec& J = this->imageGradientTimesJacobian[i];

        this->incrementWarpedImage.push_back(this->warpedImage[i] + arma::dot(J.t(), increment));

      }

    }

    /*--------------------------------------------------------------------------*/

  };

}

#endif
