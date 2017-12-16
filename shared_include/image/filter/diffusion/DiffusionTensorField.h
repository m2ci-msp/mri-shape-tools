#ifndef __DIFFUSION_TENSOR_FIELD_H__
#define __DIFFUSION_TENSOR_FIELD_H__

#include <cmath>

#include <armadillo>

#include "image/filter/diffusion/DiffusionSettings.h"
#include "image/filter/diffusion/StructureTensorField.h"

#include "image/ImageAccess.h"
#include "image/ImageCreate.h"

class DiffusionTensorField{

public:

  /*--------------------------------------------------------------------------*/

  DiffusionTensorField(
                       const StructureTensorField& structureTensorField,
                       const DiffusionSettings& settings
                       ) :

    // dimensions
    nx(structureTensorField.nx),
    ny(structureTensorField.ny),
    nz(structureTensorField.nz),
    // access to diffusion tensor
    D11(ImageAccess(D11Data)),
    D22(ImageAccess(D22Data)),
    D33(ImageAccess(D33Data)),
    D12(ImageAccess(D12Data)),
    D13(ImageAccess(D13Data)),
    D23(ImageAccess(D23Data)),
    // structure tensor field
    structureTensors(structureTensorField),
    // settings
    settings(settings) {

    init_diffusion_tensors();

  }

  /*--------------------------------------------------------------------------*/

  void update() {

    update_diffusion_tensors();

  }

  /*--------------------------------------------------------------------------*/

  const int& nx;
  const int& ny;
  const int& nz;

  /*--------------------------------------------------------------------------*/

  ImageAccess D11;
  ImageAccess D22;
  ImageAccess D33;
  ImageAccess D12;
  ImageAccess D13;
  ImageAccess D23;

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void init_diffusion_tensors() {

    ImageCreate(this->D11Data).with_dimension(this->nx, this->ny, this->nz);
    ImageCreate(this->D22Data).with_dimension(this->nx, this->ny, this->nz);
    ImageCreate(this->D33Data).with_dimension(this->nx, this->ny, this->nz);

    ImageCreate(this->D12Data).with_dimension(this->nx, this->ny, this->nz);
    ImageCreate(this->D13Data).with_dimension(this->nx, this->ny, this->nz);
    ImageCreate(this->D23Data).with_dimension(this->nx, this->ny, this->nz);

  }

  /*--------------------------------------------------------------------------*/

  void update_diffusion_tensors() {

    // squared contrast parameter
    const double help = pow(this->settings.contrastLambda, 2.);

    for(int x = 0; x < nx; ++x) {
      for(int y = 0; y < ny; ++y) {
        for(int z = 0; z < nz; ++z) {

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

          this->D11.at_grid(x, y, z) = diffusionTensor.at(0, 0);
          this->D22.at_grid(x, y, z) = diffusionTensor.at(1, 1);
          this->D33.at_grid(x, y, z) = diffusionTensor.at(2, 2);
          this->D12.at_grid(x, y, z) = diffusionTensor.at(0, 1);
          this->D13.at_grid(x, y, z) = diffusionTensor.at(0, 2);
          this->D23.at_grid(x, y, z) = diffusionTensor.at(1, 2);

        } // end for z
      } // end for y
    } // end for x

  }

  /*--------------------------------------------------------------------------*/

  const StructureTensorField& structureTensors;
  const DiffusionSettings& settings;

  /*--------------------------------------------------------------------------*/

  ImageData D11Data;
  ImageData D22Data;
  ImageData D33Data;
  ImageData D12Data;
  ImageData D13Data;
  ImageData D23Data;

  /*--------------------------------------------------------------------------*/

};
#endif
