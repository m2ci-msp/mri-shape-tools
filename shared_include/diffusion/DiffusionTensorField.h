#ifndef __DIFFUSION_TENSOR_FIELD_H__
#define __DIFFUSION_TENSOR_FIELD_H__

#include "diffusion/DiffusionSettings.h"

#include "image/Image.h"
#include "image/ImageData.h"

class DiffusionTensorField{

public:

  /*--------------------------------------------------------------------------*/

  DiffusionTensorField(
    const ImageData& data,
    const DiffusionSettings& settings
    ) : data(data), settings(settings) {

    const int& nx = this->data.get_nx();
    const int& ny = this->data.get_ny();
    const int& nz = this->data.get_nz();

    this->J11 = Image(ImageData(nx, ny, nz));
    this->J22 = Image(ImageData(nx, ny, nz));
    this->J33 = Image(ImageData(nx, ny, nz));
    this->J12 = Image(ImageData(nx, ny, nz));
    this->J13 = Image(ImageData(nx, ny, nz));
    this->J23 = Image(ImageData(nx, ny, nz));

    this->D11 = ImageData(nx, ny, nz);
    this->D22 = ImageData(nx, ny, nz);
    this->D33 = ImageData(nx, ny, nz);
    this->D12 = ImageData(nx, ny, nz);
    this->D13 = ImageData(nx, ny, nz);
    this->D23 = ImageData(nx, ny, nz);

  }

  /*--------------------------------------------------------------------------*/

  void update() {

    update_structure_tensors();
    update_diffusion_tensors();

  }

  /*--------------------------------------------------------------------------*/

  int get_nx() const { return this->data.get_nx(); }
  int get_ny() const { return this->data.get_ny(); }
  int get_nz() const { return this->data.get_nz(); }

  /*--------------------------------------------------------------------------*/

  ImageData D11;
  ImageData D22;
  ImageData D33;
  ImageData D12;
  ImageData D13;
  ImageData D23;

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void update_structure_tensors() {

    const int& nx = this->data.get_nx();
    const int& ny = this->data.get_ny();
    const int& nz = this->data.get_nz();

    Image smoothed(this->data);
    smoothed.smooth().gaussian(this->settings.presmoothSigma);
    smoothed.data().mirror();

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {

          const double dx = smoothed.analysis().derivative_x(i, j, k);
          const double dy = smoothed.analysis().derivative_y(i, j, k);
          const double dz = smoothed.analysis().derivative_z(i, j, k);

          J11.data().at(i, j, k) = dx * dx;
          J12.data().at(i, j, k) = dx * dy;
          J13.data().at(i, j, k) = dx * dz;
          J22.data().at(i, j, k) = dy * dy;
          J23.data().at(i, j, k) = dy * dz;
          J33.data().at(i, j, k) = dz * dz;

        } // end for k
      } // end for j
    } // end for i

  } // end update_structure_tensors

  /*--------------------------------------------------------------------------*/

  void update_diffusion_tensors() {

    this->J11.smooth().gaussian(this->settings.integrationRho);
    this->J22.smooth().gaussian(this->settings.integrationRho);
    this->J33.smooth().gaussian(this->settings.integrationRho);
    this->J12.smooth().gaussian(this->settings.integrationRho);
    this->J13.smooth().gaussian(this->settings.integrationRho);
    this->J23.smooth().gaussian(this->settings.integrationRho);

    const int& nx = this->data.get_nx();
    const int& ny = this->data.get_ny();
    const int& nz = this->data.get_nz();

    // squared contrast parameter
    const double help = pow(this->settings.contrastLambda, 2.);

    for(int x = 0; x < nx; ++x) {
      for(int y = 0; y < ny; ++y) {
        for(int z = 0; z < nz; ++z) {

          const double& j11 = this->J11.data().at(x, y, z);
          const double& j22 = this->J22.data().at(x, y, z);
          const double& j33 = this->J33.data().at(x, y, z);
          const double& j12 = this->J12.data().at(x, y, z);
          const double& j13 = this->J13.data().at(x, y, z);
          const double& j23 = this->J23.data().at(x, y, z);

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

          D11.at(x, y, z) = diffusionTensor.at(0, 0);
          D22.at(x, y, z) = diffusionTensor.at(1, 1);
          D33.at(x, y, z) = diffusionTensor.at(2, 2);
          D12.at(x, y, z) = diffusionTensor.at(0, 1);
          D13.at(x, y, z) = diffusionTensor.at(0, 2);
          D23.at(x, y, z) = diffusionTensor.at(1, 2);

        } // end for z
      } // end for y
    } // end for x

  }

  /*--------------------------------------------------------------------------*/

  const ImageData& data;
  const DiffusionSettings& settings;

  /*--------------------------------------------------------------------------*/

  Image J11;
  Image J22;
  Image J33;
  Image J12;
  Image J13;
  Image J23;

  /*--------------------------------------------------------------------------*/

};
#endif
