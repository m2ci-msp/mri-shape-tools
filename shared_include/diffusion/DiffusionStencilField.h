#ifndef __DIFFUSION_STENCIL_FIELD_H__
#define __DIFFUSION_STENCIL_FIELD_H__

#include "image/ImageData.h"
#include "diffusion/DiffusionTensorField.h"

class DiffusionStencilField{

public:

  /*--------------------------------------------------------------------------*/

  DiffusionStencilField(const DiffusionTensorField& field) : field(field) {

    const int& nx = this->field.get_nx();
    const int& ny = this->field.get_ny();
    const int& nz = this->field.get_nz();

    this->X0Y0Z0 = ImageData(nx, ny, nz);
    this->XMYMZ0 = ImageData(nx, ny, nz);
    this->XMYPZ0 = ImageData(nx, ny, nz);
    this->XPYMZ0 = ImageData(nx, ny, nz);
    this->XPYPZ0 = ImageData(nx, ny, nz);

    this->XMY0ZM = ImageData(nx, ny, nz);
    this->XMY0ZP = ImageData(nx, ny, nz);
    this->XPY0ZM = ImageData(nx, ny, nz);
    this->XPY0ZP = ImageData(nx, ny, nz);

    this->X0YMZM = ImageData(nx, ny, nz);
    this->X0YMZP = ImageData(nx, ny, nz);
    this->X0YPZM = ImageData(nx, ny, nz);
    this->X0YPZP = ImageData(nx, ny, nz);

    this->XMY0Z0 = ImageData(nx, ny, nz);
    this->XPY0Z0 = ImageData(nx, ny, nz);
    this->X0YMZ0 = ImageData(nx, ny, nz);
    this->X0YPZ0 = ImageData(nx, ny, nz);
    this->X0Y0ZM = ImageData(nx, ny, nz);
    this->X0Y0ZP = ImageData(nx, ny, nz);

  }

  /*--------------------------------------------------------------------------*/

  void update() {

    const int& nx = field.D11.get_nx();
    const int& ny = field.D11.get_ny();
    const int& nz = field.D11.get_nz();

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
          const double XMYMZ0 = xm * ym *  0.25 * ( field.D12.at(i-xm, j, k) + field.D12.at(i, j-ym, k) );
          const double XMYPZ0 = xm * yp * -0.25 * ( field.D12.at(i-xm, j, k) + field.D12.at(i, j+yp, k) );
          const double XPYPZ0 = xp * yp *  0.25 * ( field.D12.at(i+xp, j, k) + field.D12.at(i, j+yp, k) );
          const double XPYMZ0 = xp * ym * -0.25 * ( field.D12.at(i+xp, j, k) + field.D12.at(i, j-ym, k) );

          /* weights from mixed x-z derivatives */
          const double XMY0ZM = xm * zm *  0.25 * ( field.D13.at(i-xm, j, k) + field.D13.at(i, j, k-zm) ); 
          const double XMY0ZP = xm * zp * -0.25 * ( field.D13.at(i-xm, j, k) + field.D13.at(i, j, k+zp) );
          const double XPY0ZP = xp * zp *  0.25 * ( field.D13.at(i+xp, j, k) + field.D13.at(i, j, k+zp) );
          const double XPY0ZM = xp * zm * -0.25 * ( field.D13.at(i+xp, j, k) + field.D13.at(i, j, k-zm) );

          /* weights from mixed y-z derivatives */
          const double X0YMZM = ym * zm *  0.25 * ( field.D23.at(i, j-ym, k) + field.D23.at(i, j, k-zm) );
          const double X0YMZP = ym * zp * -0.25 * ( field.D23.at(i, j-ym, k) + field.D23.at(i, j, k+zp) );
          const double X0YPZP = yp * zp *  0.25 * ( field.D23.at(i, j+yp, k) + field.D23.at(i, j, k+zp) );
          const double X0YPZM = yp * zm * -0.25 * ( field.D23.at(i, j+yp, k) + field.D23.at(i, j, k-zm) );

          /* weights from homogeneous derivatives */
          const double XPY0Z0 = xp * 0.5 * ( field.D11.at(i+zp, j, k) + field.D11.at(i, j, k) );
          const double XMY0Z0 = xm * 0.5 * ( field.D11.at(i-zm, j, k) + field.D11.at(i, j, k) );
          const double X0YPZ0 = yp * 0.5 * ( field.D22.at(i, j+yp, k) + field.D22.at(i, j, k) );
          const double X0YMZ0 = ym * 0.5 * ( field.D22.at(i, j-ym, k) + field.D22.at(i, j, k) ); 
          const double X0Y0ZP = zp * 0.5 * ( field.D33.at(i, j, k+zp) + field.D33.at(i, j, k) );
          const double X0Y0ZM = zm * 0.5 * ( field.D33.at(i, j, k-zm) + field.D33.at(i, j, k) );

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

          this->X0Y0Z0.at(i, j, k) = X0Y0Z0;

          this->XMYMZ0.at(i, j, k) = XMYMZ0; 
          this->XMYPZ0.at(i, j, k) = XMYPZ0;
          this->XPYMZ0.at(i, j, k) = XPYMZ0;
          this->XPYPZ0.at(i, j, k) = XPYPZ0; 

          this->XMY0ZM.at(i, j, k) = XMY0ZM;
          this->XMY0ZP.at(i, j, k) = XMY0ZP;
          this->XPY0ZM.at(i, j, k) = XPY0ZM;
          this->XPY0ZP.at(i, j, k) = XPY0ZP;

          this->X0YMZM.at(i, j, k) = X0YMZM;
          this->X0YMZP.at(i, j, k) = X0YMZP;
          this->X0YPZM.at(i, j, k) = X0YPZM;
          this->X0YPZP.at(i, j, k) = X0YPZP;

          this->XMY0Z0.at(i, j, k) = XMY0Z0;
          this->XPY0Z0.at(i, j, k) = XPY0Z0;
          this->X0YMZ0.at(i, j, k) = X0YMZ0;
          this->X0YPZ0.at(i, j, k) = X0YPZ0;
          this->X0Y0ZM.at(i, j, k) = X0Y0ZM;
          this->X0Y0ZP.at(i, j, k) = X0Y0ZP;

        } // end for j

      } // end for i

    } // end for k
    
  }

  /*--------------------------------------------------------------------------*/

  ImageData X0Y0Z0;

  ImageData XMYMZ0;
  ImageData XMYPZ0;
  ImageData XPYMZ0;
  ImageData XPYPZ0;

  ImageData XMY0ZM;
  ImageData XMY0ZP;
  ImageData XPY0ZM;
  ImageData XPY0ZP;

  ImageData X0YMZM;
  ImageData X0YMZP;
  ImageData X0YPZM;
  ImageData X0YPZP;

  ImageData XMY0Z0;
  ImageData XPY0Z0;
  ImageData X0YMZ0;
  ImageData X0YPZ0;
  ImageData X0Y0ZM;
  ImageData X0Y0ZP;
 
  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  const DiffusionTensorField& field;

  /*--------------------------------------------------------------------------*/

};
#endif
