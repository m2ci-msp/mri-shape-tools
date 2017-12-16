#ifndef __DIFFUSION_STENCIL_FIELD_H__
#define __DIFFUSION_STENCIL_FIELD_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/filter/diffusion/DiffusionTensorField.h"

class DiffusionStencilField{

public:

  /*--------------------------------------------------------------------------*/

  DiffusionStencilField(
                        const DiffusionTensorField& diffusionTensors
                        ) :
    // dimensions
    nx(diffusionTensors.nx),
    ny(diffusionTensors.ny),
    nz(diffusionTensors.nz),
    // stencil access
    X0Y0Z0(ImageAccess(X0Y0Z0Data)),

    XMYMZ0(ImageAccess(XMYMZ0Data)),
    XMYPZ0(ImageAccess(XMYPZ0Data)),
    XPYMZ0(ImageAccess(XPYMZ0Data)),
    XPYPZ0(ImageAccess(XPYPZ0Data)),

    XMY0ZM(ImageAccess(XMY0ZMData)),
    XMY0ZP(ImageAccess(XMY0ZPData)),
    XPY0ZM(ImageAccess(XPY0ZMData)),
    XPY0ZP(ImageAccess(XPY0ZPData)),

    X0YMZM(X0YMZMData),
    X0YMZP(X0YMZPData),
    X0YPZM(X0YPZMData),
    X0YPZP(X0YPZPData),

    XMY0Z0(ImageAccess(XMY0Z0Data)),
    XPY0Z0(ImageAccess(XPY0Z0Data)),
    X0YMZ0(ImageAccess(X0YMZ0Data)),
    X0YPZ0(ImageAccess(X0YPZ0Data)),
    X0Y0ZM(ImageAccess(X0Y0ZMData)),
    X0Y0ZP(ImageAccess(X0Y0ZPData)),
    // diffusion tensors
    field(diffusionTensors)

  {

    init_stencils();

  }

  /*--------------------------------------------------------------------------*/

  void update() {

    for(int k = 0; k < nz; ++k) {

      const int zp = ( k != nz - 1 )? 1 : 0;
      const int zm = ( k != 0      )? 1 : 0;

      for(int i = 0; i < nx; ++i) {

        const int xp = ( i != nx - 1 )? 1 : 0;
        const int xm = ( i != 0      )? 1 : 0;

        for(int j = 0; j < ny; ++j) {

          /* set neighborhood indicators */
          const int yp = ( j != ny - 1 )? 1: 0;
          const int ym = ( j != 0      )? 1: 0;

          /* weights from mixed x-y derivatives */
          const double XMYMZ0 = xm * ym *  0.25 * ( field.D12.at_grid(i-xm, j, k) + field.D12.at_grid(i, j-ym, k) );
          const double XMYPZ0 = xm * yp * -0.25 * ( field.D12.at_grid(i-xm, j, k) + field.D12.at_grid(i, j+yp, k) );
          const double XPYPZ0 = xp * yp *  0.25 * ( field.D12.at_grid(i+xp, j, k) + field.D12.at_grid(i, j+yp, k) );
          const double XPYMZ0 = xp * ym * -0.25 * ( field.D12.at_grid(i+xp, j, k) + field.D12.at_grid(i, j-ym, k) );

          /* weights from mixed x-z derivatives */
          const double XMY0ZM = xm * zm *  0.25 * ( field.D13.at_grid(i-xm, j, k) + field.D13.at_grid(i, j, k-zm) );
          const double XMY0ZP = xm * zp * -0.25 * ( field.D13.at_grid(i-xm, j, k) + field.D13.at_grid(i, j, k+zp) );
          const double XPY0ZP = xp * zp *  0.25 * ( field.D13.at_grid(i+xp, j, k) + field.D13.at_grid(i, j, k+zp) );
          const double XPY0ZM = xp * zm * -0.25 * ( field.D13.at_grid(i+xp, j, k) + field.D13.at_grid(i, j, k-zm) );

          /* weights from mixed y-z derivatives */
          const double X0YMZM = ym * zm *  0.25 * ( field.D23.at_grid(i, j-ym, k) + field.D23.at_grid(i, j, k-zm) );
          const double X0YMZP = ym * zp * -0.25 * ( field.D23.at_grid(i, j-ym, k) + field.D23.at_grid(i, j, k+zp) );
          const double X0YPZP = yp * zp *  0.25 * ( field.D23.at_grid(i, j+yp, k) + field.D23.at_grid(i, j, k+zp) );
          const double X0YPZM = yp * zm * -0.25 * ( field.D23.at_grid(i, j+yp, k) + field.D23.at_grid(i, j, k-zm) );

          /* weights from homogeneous derivatives */
          const double XPY0Z0 = xp * 0.5 * ( field.D11.at_grid(i+zp, j, k) + field.D11.at_grid(i, j, k) );
          const double XMY0Z0 = xm * 0.5 * ( field.D11.at_grid(i-zm, j, k) + field.D11.at_grid(i, j, k) );
          const double X0YPZ0 = yp * 0.5 * ( field.D22.at_grid(i, j+yp, k) + field.D22.at_grid(i, j, k) );
          const double X0YMZ0 = ym * 0.5 * ( field.D22.at_grid(i, j-ym, k) + field.D22.at_grid(i, j, k) );
          const double X0Y0ZP = zp * 0.5 * ( field.D33.at_grid(i, j, k+zp) + field.D33.at_grid(i, j, k) );
          const double X0Y0ZM = zm * 0.5 * ( field.D33.at_grid(i, j, k-zm) + field.D33.at_grid(i, j, k) );

          /* central weight */
          const double X0Y0Z0 =
            - (
               /* x-y mixed weights */
               XMYMZ0 +
               XMYPZ0 +
               XPYMZ0 +
               XPYPZ0 +
               /* x-z mixed weights */
               XMY0ZM +
               XMY0ZP +
               XPY0ZM +
               XPY0ZP +
               /* y-z mixed weights */
               X0YMZM +
               X0YMZP +
               X0YPZM +
               X0YPZP +
               /* homogeneous weights */
               XMY0Z0 +
               XPY0Z0 +
               X0YMZ0 +
               X0YPZ0 +
               X0Y0ZM +
               X0Y0ZP
               );

          this->X0Y0Z0.at_grid(i, j, k) = X0Y0Z0;

          this->XMYMZ0.at_grid(i, j, k) = XMYMZ0;
          this->XMYPZ0.at_grid(i, j, k) = XMYPZ0;
          this->XPYMZ0.at_grid(i, j, k) = XPYMZ0;
          this->XPYPZ0.at_grid(i, j, k) = XPYPZ0;

          this->XMY0ZM.at_grid(i, j, k) = XMY0ZM;
          this->XMY0ZP.at_grid(i, j, k) = XMY0ZP;
          this->XPY0ZM.at_grid(i, j, k) = XPY0ZM;
          this->XPY0ZP.at_grid(i, j, k) = XPY0ZP;

          this->X0YMZM.at_grid(i, j, k) = X0YMZM;
          this->X0YMZP.at_grid(i, j, k) = X0YMZP;
          this->X0YPZM.at_grid(i, j, k) = X0YPZM;
          this->X0YPZP.at_grid(i, j, k) = X0YPZP;

          this->XMY0Z0.at_grid(i, j, k) = XMY0Z0;
          this->XPY0Z0.at_grid(i, j, k) = XPY0Z0;
          this->X0YMZ0.at_grid(i, j, k) = X0YMZ0;
          this->X0YPZ0.at_grid(i, j, k) = X0YPZ0;
          this->X0Y0ZM.at_grid(i, j, k) = X0Y0ZM;
          this->X0Y0ZP.at_grid(i, j, k) = X0Y0ZP;

        } // end for j

      } // end for i

    } // end for k

  }

  /*--------------------------------------------------------------------------*/

  const int& nx;
  const int& ny;
  const int& nz;

  ImageAccess X0Y0Z0;

  ImageAccess XMYMZ0;
  ImageAccess XMYPZ0;
  ImageAccess XPYMZ0;
  ImageAccess XPYPZ0;

  ImageAccess XMY0ZM;
  ImageAccess XMY0ZP;
  ImageAccess XPY0ZM;
  ImageAccess XPY0ZP;

  ImageAccess X0YMZM;
  ImageAccess X0YMZP;
  ImageAccess X0YPZM;
  ImageAccess X0YPZP;

  ImageAccess XMY0Z0;
  ImageAccess XPY0Z0;
  ImageAccess X0YMZ0;
  ImageAccess X0YPZ0;
  ImageAccess X0Y0ZM;
  ImageAccess X0Y0ZP;

  /*--------------------------------------------------------------------------*/

private:

  void init_stencils() {

    ImageCreate(this->X0Y0Z0Data).empty_image(this->nx, this->ny, this->nz);

    ImageCreate(this->XMYMZ0Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->XMYPZ0Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->XPYMZ0Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->XPYPZ0Data).empty_image(this->nx, this->ny, this->nz);

    ImageCreate(this->XMY0ZMData).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->XMY0ZPData).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->XPY0ZMData).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->XPY0ZPData).empty_image(this->nx, this->ny, this->nz);

    ImageCreate(this->X0YMZMData).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->X0YMZPData).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->X0YPZMData).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->X0YPZPData).empty_image(this->nx, this->ny, this->nz);

    ImageCreate(this->XMY0Z0Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->XPY0Z0Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->X0YMZ0Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->X0YPZ0Data).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->X0Y0ZMData).empty_image(this->nx, this->ny, this->nz);
    ImageCreate(this->X0Y0ZPData).empty_image(this->nx, this->ny, this->nz);

  }


  /*--------------------------------------------------------------------------*/

  const DiffusionTensorField& field;

  /*--------------------------------------------------------------------------*/
  ImageData X0Y0Z0Data;

  ImageData XMYMZ0Data;
  ImageData XMYPZ0Data;
  ImageData XPYMZ0Data;
  ImageData XPYPZ0Data;

  ImageData XMY0ZMData;
  ImageData XMY0ZPData;
  ImageData XPY0ZMData;
  ImageData XPY0ZPData;

  ImageData X0YMZMData;
  ImageData X0YMZPData;
  ImageData X0YPZMData;
  ImageData X0YPZPData;

  ImageData XMY0Z0Data;
  ImageData XPY0Z0Data;
  ImageData X0YMZ0Data;
  ImageData X0YPZ0Data;
  ImageData X0Y0ZMData;
  ImageData X0Y0ZPData;

};
#endif
