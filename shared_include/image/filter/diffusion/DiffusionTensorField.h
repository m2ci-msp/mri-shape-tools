#ifndef __DIFFUSION_TENSOR_FIELD_H__
#define __DIFFUSION_TENSOR_FIELD_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageCreate.h"

class DiffusionTensorField{

public:

  /*--------------------------------------------------------------------------*/

  DiffusionTensorField(
                       const int& nx,
                       const int& ny,
                       const int& nz,
                       const double& hx,
                       const double& hy,
                       const double& hz
                       ) :

    // dimensions
    nx(nx),
    ny(ny),
    nz(nz),
    // spacings
    hx(hx),
    hy(hy),
    hz(hz),
    // access to diffusion tensor
    D11(ImageAccess(D11Data)),
    D22(ImageAccess(D22Data)),
    D33(ImageAccess(D33Data)),
    D12(ImageAccess(D12Data)),
    D13(ImageAccess(D13Data)),
    D23(ImageAccess(D23Data)) {

    init_diffusion_tensors();

  }

  /*--------------------------------------------------------------------------*/

  const int& nx;
  const int& ny;
  const int& nz;

  const double& hx;
  const double& hy;
  const double& hz;

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

    ImageCreate(this->D11Data).with_dimension(this->nx, this->ny, this->nz).empty_image();
    ImageCreate(this->D22Data).with_dimension(this->nx, this->ny, this->nz).empty_image();
    ImageCreate(this->D33Data).with_dimension(this->nx, this->ny, this->nz).empty_image();

    ImageCreate(this->D12Data).with_dimension(this->nx, this->ny, this->nz).empty_image();
    ImageCreate(this->D13Data).with_dimension(this->nx, this->ny, this->nz).empty_image();
    ImageCreate(this->D23Data).with_dimension(this->nx, this->ny, this->nz).empty_image();

  }

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
