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
    const std::vector<arma::vec>& originalLocations;

    // image data
    Image image;

    // values depending on the current transformation matrix
    std::vector<arma::vec> transformedLocations;

    std::vector<bool> locationValid;

    std::vector<double> deformedTemplate;

    // transformation matrix
    Transformation transformationMatrix;

    /*--------------------------------------------------------------------------*/

  public:

    DeformedTemplate(
                  const Image& image,
                  const std::vector<arma::vec>& originalLocations
                  ) : image(image), originalLocations(originalLocations) {

    }

    /*--------------------------------------------------------------------------*/

    void compute(const Transformation& transformation) {

      this->transformationMatrix = transformation;

      compute_transformed_locations();

      compute_location_validity();

      compute_deformed_image();

    }

    /*--------------------------------------------------------------------------*/

    const std::vector<arma::vec>& get_transformed_locations() const {

      return this->transformedLocations;

    }

    /*--------------------------------------------------------------------------*/

    const std::vector<bool>& get_location_valid() const {

      return this->validLocation;

    }

    /*--------------------------------------------------------------------------*/

    const std::vector<double>& get_deformed_template() const {

      return this->deformedTemplate;

    }

    /*--------------------------------------------------------------------------*/

    const Transformation& get_transformation() const {

      return this->transformationMatrix;

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

        const bool valid = ( position(0) < this->image.info().get_nx() &&
                             position(0) >= 0 &&
                             position(1) < this->image.info().get_ny() &&
                             position(1) >= 0 &&
                             position(2) < this->image.info().get_nz() &&
                             position(2) >= 0 );

        this->locationValid.push_back(valid);

      }

    }

    /*--------------------------------------------------------------------------*/

    // compute the warped image: this is the current deformed template
    void compute_deformed_template() {

      this->deformedTemplate.clear();

      for(size_t i = 0; i < this->transformedLocations.size(); ++i) {

        if(this->locationValid[i] == false) {

          this->deformedTemplate.push_back(0.);

          continue;

        }

        const arma::vec& transformedLocation = this->transformedLocations[i];

        const double warped = this->image.interpolate().at_coordinate(transformedLocation(0),
                                                                              transformedLocation(1),
                                                                              transformedLocation(2)
                                                                              );
        this->deformedTemplate.push_back(warped);

      } // end for i

    }

    /*--------------------------------------------------------------------------*/

  };

}

#endif
