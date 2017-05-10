#ifndef __DIFFUSION_H__
#define __DIFFUSION_H__

#include "image/Image.h"
#include "diffusion/DiffusionSettings.h"
#include "diffusion/DiffusionTensorField.h"
#include "diffusion/DiffusionStencilField.h"

class Diffusion{

public:

  /*--------------------------------------------------------------------------*/

  Diffusion(
    const Image& image,
    const DiffusionSettings& settings
    ) : image(image), settings(settings) {

    this->image.data().set_boundary_sizes(1, 1, 1);

    this->tensorField =
      new DiffusionTensorField(this->image.data(), this->settings);

    this->stencilField =
      new DiffusionStencilField(*(this->tensorField));

  }

  /*--------------------------------------------------------------------------*/

  ~Diffusion() {
    delete this->tensorField;
    delete this->stencilField;
  }
  /*--------------------------------------------------------------------------*/

  Image get_result() {

    for(int i = 0; i < this->settings.timeSteps; ++i) {

      this->tensorField->update();
      this->stencilField->update();

      perform_step();

    }

    return this->image;

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void perform_step() {

    const int& nx = this->image.data().get_nx();
    const int& ny = this->image.data().get_ny();
    const int& nz = this->image.data().get_nz();

    ImageData copy = this->image.data();
    ImageData& data = this->image.data();

    for( int k = 0; k < nz; ++k) {

      for(int i = 0; i < nx; ++i) {

        for( int j = 0; j < ny; ++j) {

          const double weightedNeighbors =

            this->stencilField->X0Y0Z0.at(i, j, k) * copy.at(i, j, k) +

            this->stencilField->XMYMZ0.at(i, j, k) * copy.at(i-1, j-1, k) +
            this->stencilField->XMYPZ0.at(i, j, k) * copy.at(i-1, j+1, k) +
            this->stencilField->XPYMZ0.at(i, j, k) * copy.at(i+1, j-1, k) +
            this->stencilField->XPYPZ0.at(i, j, k) * copy.at(i+1, j+1, k) +

            this->stencilField->XMY0ZM.at(i, j, k) * copy.at(i-1, j, k-1) +
            this->stencilField->XMY0ZP.at(i, j, k) * copy.at(i-1, j, k+1) +
            this->stencilField->XPY0ZM.at(i, j, k) * copy.at(i+1, j, k-1) +
            this->stencilField->XPY0ZP.at(i, j, k) * copy.at(i+1, j, k+1) +

            this->stencilField->X0YMZM.at(i, j, k) * copy.at(i, j-1, k-1) +
            this->stencilField->X0YMZP.at(i, j, k) * copy.at(i, j-1, k+1) +
            this->stencilField->X0YPZM.at(i, j, k) * copy.at(i, j+1, k-1) +
            this->stencilField->X0YPZP.at(i, j, k) * copy.at(i, j+1, k+1) +

            this->stencilField->XMY0Z0.at(i, j, k) * copy.at(i-1, j, k) +
            this->stencilField->XPY0Z0.at(i, j, k) * copy.at(i+1, j, k) +
            this->stencilField->X0YMZ0.at(i, j, k) * copy.at(i, j-1, k) +
            this->stencilField->X0YPZ0.at(i, j, k) * copy.at(i, j+1, k) +
            this->stencilField->X0Y0ZM.at(i, j, k) * copy.at(i, j, k-1) +
            this->stencilField->X0Y0ZP.at(i, j, k) * copy.at(i, j, k+1);

          data.at(i, j, k) += this->settings.stepSize * weightedNeighbors;

        } // end for j
      } // end for i
    } // end for k

  }

 /*--------------------------------------------------------------------------*/

  Image image;
  const DiffusionSettings& settings;

  DiffusionTensorField* tensorField;
  DiffusionStencilField* stencilField;

  /*--------------------------------------------------------------------------*/
};

#endif
