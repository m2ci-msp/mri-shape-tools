#ifndef __INCREMENTALLY_DEFORMED_TEMPLATE_H_
#define __INCREMENTALLY_DEFORMED_TEMPLATE_H_

#include <vector>

#include <armadillo>

#include "image/Image.h"
#include "matrix/Transformation.h"
#include "DeformedTemplate.h"

namespace lucasKanade{

  class IncrementallyDeformedTemplate{

  private:

    // input
    Image image;

    const std::vector<arma::vec>& originalLocations;

    const DeformedTemplate& deformedTemplate;

    arma::vec increment;

    // immediate results
    Image imageX;
    Image imageY;
    Image imageZ;

    std::vector<arma::mat> imageGradientTimesJacobian;

    // output
    std::vector<double> incrementallyDeformedTemplate;

  public:

    /*--------------------------------------------------------------------------*/

    IncrementallyDeformedTemplate(
                                  const Image& image,
                                  const std::vector<arma::vec>& originalLocations,
                                  const DeformedTemplate& deformedTemplate
                                  ) : image(image),
                                      originalLocations(originalLocations),
                                      deformedTemplate(deformedTemplate) {
      compute_image_derivatives();

    }

    /*--------------------------------------------------------------------------*/

    void compute(const arma::vec& increment) {

      this->increment = increment;

      compute_image_gradient_times_jacobian();

      compute_incrementally_deformed_template();

    }

    /*--------------------------------------------------------------------------*/

    const std::vector<arma::mat>& get_image_gradient_times_jacobian() const {

      return this->imageGradientTimesJacobian;

    }

    /*--------------------------------------------------------------------------*/

    const std::vector<double>& get_incrementally_deformed_template() const {

      return this->incrementallyDeformedTemplate;

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    void compute_image_derivatives() {

      this->image.boundary().change(1, 1, 1);
      this->image.mirror().all();

      this->imageX = this->image;
      this->imageY = this->image;
      this->imageZ = this->image;

      for(int i = 0; i < this->image.info().get_nx(); ++i) {
        for(int j = 0; j < this->image.info().get_ny(); ++j) {
          for(int k = 0; k < this->image.info().get_nz(); ++k) {

            this->imageX.access().at_grid(i, j, k) =
              this->image.calculus().dx(i, j, k);

            this->imageY.access().at_grid(i, j, k) =
              this->image.calculus().dy(i, j, k);

            this->imageZ.access().at_grid(i, j, k) =
              this->image.calculus().dz(i, j, k);

          }

        }

      }

      this->imageX.mirror().all();
      this->imageY.mirror().all();
      this->imageZ.mirror().all();

    }

    /*--------------------------------------------------------------------------*/

    // compute the jacobian of the transformation matrix
    arma::mat compute_jacobian(const double& x, const double& y, const double& z) const {

      const arma::vec p({ x, y, z });

      const Transformation& transformationMatrix = this->deformedTemplate.get_transformation();

      // compute derivatives
      const arma::vec dTx = transformationMatrix.apply_derivative_tx(p);
      const arma::vec dTy = transformationMatrix.apply_derivative_ty(p);
      const arma::vec dTz = transformationMatrix.apply_derivative_tz(p);

      const arma::vec dAlpha = transformationMatrix.apply_derivative_alpha(p);
      const arma::vec dBeta = transformationMatrix.apply_derivative_beta(p);
      const arma::vec dGamma = transformationMatrix.apply_derivative_gamma(p);

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

      const std::vector<arma::vec>& transformedLocations = this->deformedTemplate.get_transformed_locations();

      for(size_t i = 0; i < transformedLocations.size(); ++i) {

        const arma::vec& location = this->originalLocations[i];

        const arma::mat J = compute_jacobian(location(0), location(1), location(2));

        const arma::vec& transformedLocation = transformedLocations[i];

        const arma::vec imageGradient = compute_image_gradient(transformedLocation);

        const arma::mat result = (imageGradient.t() * J);

        this->imageGradientTimesJacobian.push_back(result.row(0).t());

      } // end for i

    }

    /*--------------------------------------------------------------------------*/

    arma::vec compute_image_gradient(const arma::vec& p) {

      // interpolate values
      const double dx = this->imageX.interpolate().at_coordinate(p(0), p(1), p(2));
      const double dy = this->imageY.interpolate().at_coordinate(p(0), p(1), p(2));
      const double dz = this->imageZ.interpolate().at_coordinate(p(0), p(1), p(2));

      // assemble image gradient
      const arma::vec gradient( { dx, dy, dz });

      return gradient;

    }

    /*--------------------------------------------------------------------------*/

    // use the current transformation increment to deform the warped image
    void compute_incrementally_deformed_template() {

      this->incrementallyDeformedTemplate.clear();

      const std::vector<double>& deformedTemplateValues = this->deformedTemplate.get_deformed_template();

      for(size_t i = 0; i < deformedTemplateValues.size(); ++i) {

        const arma::vec& J = this->imageGradientTimesJacobian[i];

        this->incrementallyDeformedTemplate.push_back(deformedTemplateValues[i] + arma::dot(J.t(), increment));

      }

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
