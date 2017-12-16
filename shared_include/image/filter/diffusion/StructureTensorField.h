#ifndef __STRUCTURE_TENSOR_FIELD_H__
#define __STRUCTURE_TENSOR_FIELD_H__

#include <armadillo>

#include "image/filter/GaussianFilter.h"
#include "image/filter/diffusion/DiffusionSettings.h"

#include "image/ImageAccess.h"
#include "image/ImageCreate.h"
#include "image/ImageCalculus.h"
#include "image/ImageData.h"
#include "image/ImageMirror.h"

class StructureTensorField{

public:

  StructureTensorField(
                       const ImageData& imageData,
                       const DiffusionSettings& settings
                       ) :
    // dimensions
    nx(imageData.nx),
    ny(imageData.ny),
    nz(imageData.nz),
    // structure tensor access
    J11(ImageAccess(J11Data)),
    J22(ImageAccess(J22Data)),
    J33(ImageAccess(J33Data)),
    J12(ImageAccess(J12Data)),
    J13(ImageAccess(J13Data)),
    J23(ImageAccess(J23Data)),
    // image data
    imageData(imageData),
    // diffusion settings
    settings(settings)
  {

    init_structure_tensors();

  }

  void update() {

    // create copy
    ImageData copyData = this->imageData;

    // smooth image
    GaussianFilter(copyData, this->settings.presmoothSigma).apply();

    // mirror boundaries
    ImageMirror(copyData).mirror();

    // create object for performing calculus operations
    ImageCalculus copy(copyData);

    for(int i = 0; i < this->nx; ++i) {
      for(int j = 0; j < this->ny; ++j) {
        for(int k = 0; k < this->nz; ++k) {

          arma::mat structureTensor = copy.structure_tensor(i, j, k);

          J11.at_grid(i, j, k) = structureTensor.at(0, 0);
          J22.at_grid(i, j, k) = structureTensor.at(1, 1);
          J33.at_grid(i, j, k) = structureTensor.at(2, 2);
          J12.at_grid(i, j, k) = structureTensor.at(0, 1);
          J13.at_grid(i, j, k) = structureTensor.at(0, 2);
          J23.at_grid(i, j, k) = structureTensor.at(1, 2);

        } // end for k
      } // end for j
    } // end for i

    // smooth each structure tensor entry
    GaussianFilter(this->J11Data, this->settings.integrationRho).apply();
    GaussianFilter(this->J22Data, this->settings.integrationRho).apply();
    GaussianFilter(this->J33Data, this->settings.integrationRho).apply();
    GaussianFilter(this->J12Data, this->settings.integrationRho).apply();
    GaussianFilter(this->J13Data, this->settings.integrationRho).apply();
    GaussianFilter(this->J23Data, this->settings.integrationRho).apply();

  } // end update_structure_tensors

  const int& nx;
  const int& ny;
  const int& nz;

  ImageAccess J11;
  ImageAccess J22;
  ImageAccess J33;
  ImageAccess J12;
  ImageAccess J13;
  ImageAccess J23;

private:

  void init_structure_tensors() {

    ImageCreate(this->J11Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->J22Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->J33Data).empty_image(this->nx, this->ny, this->nz);

    ImageCreate(this->J12Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->J13Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->J23Data).empty_image(this->nx, this->ny, this->nz);

  }

  const ImageData& imageData;
  const DiffusionSettings& settings;

  ImageData J11Data;
  ImageData J12Data;
  ImageData J13Data;
  ImageData J22Data;
  ImageData J23Data;
  ImageData J33Data;

};

#endif
