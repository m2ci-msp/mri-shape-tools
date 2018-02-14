#ifndef __LUCAS_KANADE_ENERGY_DATA_H__
#define __LUCAS_KANADE_ENERGY_DATA_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "image/Image.h"

#include "matrix/Transformation.h"
#include "energy/TransformationEnum.h"

namespace lucasKanade{

  class EnergyData{

  public:

    // region of interest
    int minX, minY, minZ, maxX, maxY, maxZ;

    // image serving as template
    Image templateImage;

    // target image
    Image target;

    // center of template region of interest
    arma::vec center;

    // x-derivative of target image
    Image targetX;

    // y-derivative of target image
    Image targetY;

    // z-derivative of target image
    Image targetZ;

    // current transformation parameters increment in serialized format
    vnl_vector<double> transformation;

    /*--------------------------------------------------------------------------*/

    const int transformationAmount = 6;

    /*--------------------------------------------------------------------------*/

    EnergyData(
               const Image& templateImage,
               const Image& target,
               const arma::vec& center,
               const arma::vec& radius
               )

      : templateImage(templateImage), target(target), center(center) {

      this->transformation = vnl_vector<double>(this->transformationAmount, 0.);

      compute_region_of_interest(center, radius);

      compute_image_derivatives();

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    void compute_region_of_interest(const arma::vec& center, const arma::vec& radius) {

      this->minX = center(0) - radius(0);
      this->minY = center(1) - radius(1);
      this->minZ = center(2) - radius(2);

      this->maxX = center(0) + radius(0);
      this->maxY = center(1) + radius(1);
      this->maxZ = center(2) + radius(2);

    }

    /*--------------------------------------------------------------------------*/

    void compute_image_derivatives() {

      this->target.boundary().change(1, 1, 1);
      this->target.mirror().all();

      this->targetX = this->target;
      this->targetY = this->target;
      this->targetZ = this->target;

      for(int i = 0; i < this->target.info().get_nx(); ++i) {
        for(int j = 0; j < this->target.info().get_ny(); ++j) {
          for(int k = 0; k < this->target.info().get_nz(); ++k) {

            this->targetX.access().at_grid(i, j, k) =
              this->target.calculus().dx(i, j, k);

            this->targetY.access().at_grid(i, j, k) =
              this->target.calculus().dy(i, j, k);

            this->targetZ.access().at_grid(i, j, k) =
              this->target.calculus().dz(i, j, k);

          }

        }

      }

      this->targetX.mirror().all();
      this->targetY.mirror().all();
      this->targetZ.mirror().all();

    }

    /*--------------------------------------------------------------------------*/

  };

}
#endif
