#ifndef __DIFFUSION_FILTER_H__
#define __DIFFUSION_FILTER_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/filter/diffusion/DiffusionSettings.h"
#include "image/filter/diffusion/FilterDiffusionTensorFieldBuilder.h"
#include "image/filter/diffusion/DiffusionStencilField.h"
#include "image/filter/diffusion/StructureTensorField.h"

class DiffusionFilter{

public:

  /*--------------------------------------------------------------------------*/

  DiffusionFilter(
            ImageData& imageData,
            const DiffusionSettings& settings
            ) :
    imageData(imageData),
    imageAccess(imageData),
    imageBoundary(imageData),
    settings(settings),
    structureTensors(imageData, settings),
    diffusionTensors(structureTensors, settings),
    stencils(diffusionTensors.get_field()) {

    // reset boundary size
    this->imageBoundary.change(1, 1, 1);

  }

  /*--------------------------------------------------------------------------*/

  void apply() {

    for(int i = 0; i < this->settings.timeSteps; ++i) {

      this->structureTensors.update();
      this->diffusionTensors.update();
      this->stencils.update();

      perform_step();

    }

  }

private:

  /*--------------------------------------------------------------------------*/

  void perform_step() {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    ImageData copyData = this->imageData;
    ImageAccess copy(copyData);

    for( int k = 0; k < nz; ++k) {

      for(int i = 0; i < nx; ++i) {

        for( int j = 0; j < ny; ++j) {

          const double weightedNeighbors =

            this->stencils.X0Y0Z0.at_grid(i, j, k) * copy.at_grid(i, j, k) +

            this->stencils.XMYMZ0.at_grid(i, j, k) * copy.at_grid(i-1, j-1, k) +
            this->stencils.XMYPZ0.at_grid(i, j, k) * copy.at_grid(i-1, j+1, k) +
            this->stencils.XPYMZ0.at_grid(i, j, k) * copy.at_grid(i+1, j-1, k) +
            this->stencils.XPYPZ0.at_grid(i, j, k) * copy.at_grid(i+1, j+1, k) +

            this->stencils.XMY0ZM.at_grid(i, j, k) * copy.at_grid(i-1, j, k-1) +
            this->stencils.XMY0ZP.at_grid(i, j, k) * copy.at_grid(i-1, j, k+1) +
            this->stencils.XPY0ZM.at_grid(i, j, k) * copy.at_grid(i+1, j, k-1) +
            this->stencils.XPY0ZP.at_grid(i, j, k) * copy.at_grid(i+1, j, k+1) +

            this->stencils.X0YMZM.at_grid(i, j, k) * copy.at_grid(i, j-1, k-1) +
            this->stencils.X0YMZP.at_grid(i, j, k) * copy.at_grid(i, j-1, k+1) +
            this->stencils.X0YPZM.at_grid(i, j, k) * copy.at_grid(i, j+1, k-1) +
            this->stencils.X0YPZP.at_grid(i, j, k) * copy.at_grid(i, j+1, k+1) +

            this->stencils.XMY0Z0.at_grid(i, j, k) * copy.at_grid(i-1, j, k) +
            this->stencils.XPY0Z0.at_grid(i, j, k) * copy.at_grid(i+1, j, k) +
            this->stencils.X0YMZ0.at_grid(i, j, k) * copy.at_grid(i, j-1, k) +
            this->stencils.X0YPZ0.at_grid(i, j, k) * copy.at_grid(i, j+1, k) +
            this->stencils.X0Y0ZM.at_grid(i, j, k) * copy.at_grid(i, j, k-1) +
            this->stencils.X0Y0ZP.at_grid(i, j, k) * copy.at_grid(i, j, k+1);

          this->imageAccess.at_grid(i, j, k) += this->settings.stepSize * weightedNeighbors;

        } // end for j
      } // end for i
    } // end for k

  }

  /*--------------------------------------------------------------------------*/

  ImageData& imageData;
  ImageAccess imageAccess;
  ImageBoundary imageBoundary;

  const DiffusionSettings& settings;

  StructureTensorField structureTensors;
  FilterDiffusionTensorFieldBuilder diffusionTensors;
  DiffusionStencilField stencils;

  /*--------------------------------------------------------------------------*/
};

#endif
