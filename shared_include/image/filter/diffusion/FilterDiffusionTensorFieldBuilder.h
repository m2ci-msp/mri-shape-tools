#ifndef __FILTER_DIFFUSION_TENSOR_FIELD_BUILDER_H__
#define __FILTER_DIFFUSION_TENSOR_FIELD_BUILDER_H__

#include <cmath>

#include <armadillo>

#include "image/filter/diffusion/DiffusionSettings.h"
#include "image/filter/diffusion/DiffusionTensorField.h"
#include "image/filter/diffusion/StructureTensorField.h"

#include "image/ImageAccess.h"
#include "image/ImageCreate.h"

class FilterDiffusionTensorFieldBuilder{

private:

  DiffusionTensorField tensorField;

  const StructureTensorField& structureTensors;
  const DiffusionSettings& settings;

public:

  /*--------------------------------------------------------------------------*/

  FilterDiffusionTensorFieldBuilder(
                       const StructureTensorField& structureTensorField,
                       const DiffusionSettings& settings
                       ) :

    // dimensions
    tensorField(
                structureTensorField.nx,
                structureTensorField.ny,
                structureTensorField.nz,
                structureTensorField.get_hx(),
                structureTensorField.get_hy(),
                structureTensorField.get_hz()
                ),
    structureTensors(structureTensorField),
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

  /*--------------------------------------------------------------------------*/

  void update_diffusion_tensors() {

    // squared contrast parameter
    const double help = pow(this->settings.contrastLambda, 2.);

    for(int x = 0; x < this->tensorField.nx; ++x) {
      for(int y = 0; y < this->tensorField.ny; ++y) {
        for(int z = 0; z < this->tensorField.nz; ++z) {

          const double& j11 = this->structureTensors.J11.at_grid(x, y, z);
          const double& j22 = this->structureTensors.J22.at_grid(x, y, z);
          const double& j33 = this->structureTensors.J33.at_grid(x, y, z);
          const double& j12 = this->structureTensors.J12.at_grid(x, y, z);
          const double& j13 = this->structureTensors.J13.at_grid(x, y, z);
          const double& j23 = this->structureTensors.J23.at_grid(x, y, z);

          arma::mat J({
              { j11, j12, j13 },
              { j12, j22, j23 },
              { j13, j23, j33 }
            } );

          arma::vec eigval;
          arma::mat eigvec;

          arma::eig_sym(eigval, eigvec, J, "std");

          // account for negative eigenvalues due to numerical errors
          eigval.at(0) = ( eigval.at(0) < 0. )? 0: eigval.at(0);
          eigval.at(1) = ( eigval.at(1) < 0. )? 0: eigval.at(1);
          eigval.at(2) = ( eigval.at(2) < 0. )? 0: eigval.at(2);

          /* apply perona-malik penaliser to the largest eigenvalue
           * -> enhance surface boundaries
           */
          eigval.at(2) = 1. / ( 1. + eigval.at(2) / help);
          /* set the other eigenvalues to 1 -> smooth along surface */
          eigval.at(1) = 1.;
          eigval.at(0) = 1.;

          // transform back
          arma::mat D = arma::zeros(3, 3);
          D.diag() = eigval;

          const arma::mat diffusionTensor = eigvec * D * eigvec.t();

          this->tensorField.D11.at_grid(x, y, z) = diffusionTensor.at(0, 0);
          this->tensorField.D22.at_grid(x, y, z) = diffusionTensor.at(1, 1);
          this->tensorField.D33.at_grid(x, y, z) = diffusionTensor.at(2, 2);
          this->tensorField.D12.at_grid(x, y, z) = diffusionTensor.at(0, 1);
          this->tensorField.D13.at_grid(x, y, z) = diffusionTensor.at(0, 2);
          this->tensorField.D23.at_grid(x, y, z) = diffusionTensor.at(1, 2);

        } // end for z
      } // end for y
    } // end for x

  }

};
#endif
