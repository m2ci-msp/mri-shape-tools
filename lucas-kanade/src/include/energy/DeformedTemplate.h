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

    // image data
    Image image;

    // these are the undeformed voxel coordinates of the subimage that is used as template
    const std::vector<arma::vec>& originalLocations;

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

      this->image.boundary().change(1, 1, 1);
      this->image.mirror().all();

    }

    /*--------------------------------------------------------------------------*/

    void compute(const Transformation& transformation) {

      this->transformationMatrix = transformation;

      compute_transformed_locations();

      compute_deformed_template();

    }

    /*--------------------------------------------------------------------------*/

    const std::vector<arma::vec>& get_transformed_locations() const {

      return this->transformedLocations;

    }

    /*--------------------------------------------------------------------------*/

    const std::vector<bool>& get_location_valid() const {

      return this->locationValid;

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
      this->locationValid.clear();

      for(const arma::vec& location: this->originalLocations) {

        const arma::vec transformedLocation = this->transformationMatrix.apply_matrix(location);

        if( is_valid(transformedLocation) ) {

          this->transformedLocations.push_back(transformedLocation);
          this->locationValid.push_back(true);

        }
        else {

          this->locationValid.push_back(false);

        }

      }

    }

    /*--------------------------------------------------------------------------*/

    bool is_valid(const arma::vec& position) {

      const int& nx = this->image.info().get_nx();
      const int& ny = this->image.info().get_ny();
      const int& nz = this->image.info().get_nz();

      const double& hx = this->image.info().get_hx();
      const double& hy = this->image.info().get_hy();
      const double& hz = this->image.info().get_hz();

      const bool valid = ( position(0) < nx * hx &&
                           position(0) >= 0 &&
                           position(1) < ny * hy &&
                           position(1) >= 0 &&
                           position(2) < nz * hz &&
                           position(2) >= 0 );

        return valid;

    }

    /*--------------------------------------------------------------------------*/

    // compute the warped image: this is the current deformed template
    void compute_deformed_template() {

      this->deformedTemplate.clear();

      for(size_t i = 0; i < this->transformedLocations.size(); ++i) {

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
