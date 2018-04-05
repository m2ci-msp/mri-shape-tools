#ifndef __BUILD_TEMPLATE_FROM_BOUNDING_BOX_H__
#define __BUILD_TEMPLATE_FROM_BOUNDING_BOX_H__

#include <vector>

#include <armadillo>

#include "image/Image.h"

namespace lucasKanade{

  class BuildTemplateFromBoundingBox{

  private:

    /*-------------------------------------------------------------------------*/

    const arma::vec min;
    const arma::vec max;

    const Image image;

    arma::vec center;

    std::vector<arma::vec> locations;

    /*-------------------------------------------------------------------------*/

  public:

    /*-------------------------------------------------------------------------*/

    BuildTemplateFromBoundingBox(
                                 const arma::vec& min,
                                 const arma::vec& max,
                                 const Image& image
                                 ) :
      min(min), max(max), image(image) {

      compute_locations();

      compute_rotation_center();

    }

    /*-------------------------------------------------------------------------*/

    const std::vector<arma::vec>& get_locations() const {

      return this->locations;

    }

    /*-------------------------------------------------------------------------*/

    const arma::vec& get_center() const {

      return this->center;

    }

    /*-------------------------------------------------------------------------*/

  private:

    /*-------------------------------------------------------------------------*/

    void compute_locations() {

      const double& hx = this->image.info().get_hx();
      const double& hy = this->image.info().get_hy();
      const double& hz = this->image.info().get_hz();

      this->locations.clear();

      for(double x = this->min(0); x <= this->max(0); x += hx) {
        for(double y = this->min(1); y <= this->max(1); y += hy) {
          for(double z = this->min(2); z <= this->max(2); z += hz) {

            const arma::vec location({x, y, z});

            if( is_inside_domain(location) ) {

              this->locations.push_back(location);

            }

          }
        }
      }

    }

    /*-------------------------------------------------------------------------*/

    void compute_rotation_center() {

      this->center = arma::zeros(3);

      for(const arma::vec& location: this->locations) {

        this->center += location;

      }

      this->center /= this->locations.size();

    }

    /*-------------------------------------------------------------------------*/

    bool is_inside_domain(const arma::vec& location) const {

      const int& nx = this->image.info().get_nx();
      const int& ny = this->image.info().get_ny();
      const int& nz = this->image.info().get_nz();

      const double& hx = this->image.info().get_hx();
      const double& hy = this->image.info().get_hy();
      const double& hz = this->image.info().get_hz();


      return

        ( location(0) >= 0 ) && ( location(0) < nx * hx ) &&
        ( location(1) >= 0 ) && ( location(1) < ny * hy ) &&
        ( location(2) >= 0 ) && ( location(2) < nz * hz );

    }

    /*-------------------------------------------------------------------------*/

  };

}
#endif
