#ifndef __CHAN_VESE_DIFFUSION_TENSOR_FIELD_BUILDER_H__
#define __CHAN_VESE_DIFFUSION_TENSOR_FIELD_BUILDER_H__

#include <cmath>

#include <armadillo>

#include "image/segmentation/chanvese/ChanVeseSettings.h"
#include "image/filter/diffusion/DiffusionTensorField.h"

#include "image/ImageCalculus.h"
#include "image/ImageData.h"

class ChanVeseDiffusionTensorFieldBuilder{

private:

  DiffusionTensorField tensorField;

  ImageData& levelSetData;

  const ImageCalculus calculus;

  const ChanVeseSettings& settings;

public:

  /*--------------------------------------------------------------------------*/

  ChanVeseDiffusionTensorFieldBuilder(
                       ImageData& levelSetData,
                       const ChanVeseSettings& settings
                       ) :

    // dimensions
    tensorField(
                levelSetData.nx,
                levelSetData.ny,
                levelSetData.nz,
                levelSetData.hx,
                levelSetData.hy,
                levelSetData.hz
                ),
    levelSetData(levelSetData),
    calculus(levelSetData),
    // settings
    settings(settings) {

  }

  /*--------------------------------------------------------------------------*/

  void update() {

    update_diffusion_tensors();

  }

  /*--------------------------------------------------------------------------*/

  const DiffusionTensorField& get_field() const {

    return this->tensorField;

  }

  /*--------------------------------------------------------------------------*/

private:

  double compute_regularized_L1_norm(const arma::vec& value) const {

    return sqrt(arma::dot(value, value) + this->settings.epsilon);

  }

  /*--------------------------------------------------------------------------*/

  void update_diffusion_tensors() {

    for( int i = 0; i < this->levelSetData.nx; ++i) {
      for( int j = 0; j < this->levelSetData.ny; ++j) {
        for( int k = 0; k < this->levelSetData.nz; ++k) {

          const arma::vec gradient = this->calculus.gradient(i, j, k);

          const double value = 1. / compute_regularized_L1_norm(gradient);

          this->tensorField.D11.at_grid(i, j, k) = value;
          this->tensorField.D22.at_grid(i, j, k) = value;
          this->tensorField.D33.at_grid(i, j, k) = value;
          this->tensorField.D12.at_grid(i, j, k) = 0.;
          this->tensorField.D13.at_grid(i, j, k) = 0.;
          this->tensorField.D23.at_grid(i, j, k) = 0.;


        } // end for k

      } // end for j

    } // end for i

  } // end update_diffusion_tensors

};
#endif
