#ifndef __DEFORMED_TEMPLATE_H__
#define __DEFORMED_TEMPLATE_H__

#include <vector>
#include <armadillo>

#include "image/Image.h"
#include "matrix/Transformation.h"

namespace lucasKanade{

  class DeformedTemplate{

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

    DeformedTemplate(
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

  private:

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

    // compute the warped image: this is the current deformed template
    void compute_warped_image() {

      this->warpedImage.clear();

      for(size_t i = 0; i < this->transformedLocations.size(); ++i) {

        if(this->locationValid[i] == false) {

          this->warpedImage.push_back(0.);

          continue;

        }

        const arma::vec& transformedLocation = this->transformedLocations[i];

        const double warped = this->originalImage.interpolate().at_coordinate(transformedLocation(0),
                                                                              transformedLocation(1),
                                                                              transformedLocation(2)
                                                                              );
        this->warpedImage.push_back(warped);

      } // end for i

    }

    /*--------------------------------------------------------------------------*/

  };

}

#endif
