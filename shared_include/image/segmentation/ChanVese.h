#ifndef __CHAN_VESE_H__
#define __CHAN_VESE_H__
#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/segmentation/chanvese/ChanVeseSettings.h"
#include "image/segmentation/chanvese/ChanVeseDiffusionTensorFieldBuilder.h"
#include "image/filter/diffusion/DiffusionStencilField.h"

class ChanVese{

private:

  ImageData& imageData;
  ImageAccess imageAccess;

  ImageData levelSetData;
  ImageAccess levelSetAccess;
  ImageBoundary levelSetBoundary;
  ImageMirror levelSetMirror;

  const ChanVeseSettings& settings;

  ChanVeseDiffusionTensorFieldBuilder diffusionTensors;
  DiffusionStencilField stencils;

  double regionOneAverage = 0;
  double regionTwoAverage = 0;

public:

  /*--------------------------------------------------------------------------*/

  ChanVese(
           ImageData& imageData,
           ImageData& levelSetData,
           const ChanVeseSettings& settings
           ) :
    imageData(imageData),
    imageAccess(imageData),
    levelSetData(levelSetData),
    levelSetAccess(levelSetData),
    levelSetBoundary(levelSetData),
    levelSetMirror(levelSetData),
    settings(settings),
    diffusionTensors(levelSetData, settings),
    stencils(diffusionTensors.get_field()) {

    // reset boundary size
    this->levelSetBoundary.change(1, 1, 1);

  }

  /*--------------------------------------------------------------------------*/

  void apply() {

    // perform a lagged-nonlinearity evolution
    for(int i = 0; i < this->settings.evolutionSteps; ++i) {

      // enforce von Neumann boundary conditions
      this->levelSetMirror.all();

      this->diffusionTensors.update();
      this->stencils.update();

      compute_region_averages();

      perform_step();

    }

    // segment based on level set function
    for(int i = 0; i < this->imageData.nx; ++i) {

      for(int j = 0; j < this->imageData.ny; ++j) {

        for(int k = 0; k < this->imageData.nz; ++k) {

          const double& level = this->levelSetAccess.at_grid(i, j, k);
          this->imageAccess.at_grid(i, j, k) = ( level >= 0)? 255: 0;

        }
      }
    }

  }

private:

  // computes C^inf regularized Heaviside
  double compute_regularized_heaviside(const double& value ) const {

    return 0.5 * ( 1. + 2. / M_PI * atan( value / this->settings.epsilon ) );

  }

  /*--------------------------------------------------------------------------*/

  double compute_regularized_delta(const double& value ) const {

    return 1 / (M_PI * this->settings.epsilon)  * 1. / ( 1. + pow(value / this->settings.epsilon, 2));

  }

  void compute_region_averages() {

    double sizeRegionOne = 0;
    double sizeRegionTwo = 0;

    // sums of gray values in both regions
    double sumRegionOne = 0;
    double sumRegionTwo = 0;

    for( int i = 0; i < this->levelSetData.nx; ++i) {
      for( int j = 0; j < this->levelSetData.ny; ++j) {
        for( int k = 0; k < this->levelSetData.nz; ++k) {

          const double H = compute_regularized_heaviside( this->levelSetAccess.at_grid( i, j, k ) );

          // sum up gray values and region points
          sizeRegionOne += H;
          sizeRegionTwo += 1 - H;

          sumRegionOne += H * this->imageAccess.at_grid(i, j, k);
          sumRegionTwo += ( 1 - H ) * this->imageAccess.at_grid(i, j, k);

        } // end for k
      } // end for j
    } // end for i

    // average sums
    this->regionOneAverage = sumRegionOne / sizeRegionOne;
    this->regionTwoAverage = sumRegionTwo / sizeRegionTwo;

  }

  void perform_step() {

    // create copy of current indicator function
    ImageData copyData = this->levelSetData;
    ImageAccess copy(copyData);

    // solve the current linear system of equations by using successive overrelaxation
    for( int m = 0; m < this->settings.iterationAmount; ++m) {

      for( int i = 0; i < this->levelSetData.nx; ++i) {

        for( int j = 0; j  < this->levelSetData.ny; ++j) {

          for( int k = 0; k < this->levelSetData.nz; ++k) {

            // compute regularized dirac delta function at current location of
            // old curve indicator function
            const double delta = compute_regularized_delta( copy.at_grid(i, j, k) );

            // prepare left-handside of current entry
            double lhs = 0;

            // contribution from time-dependent evolution
            lhs += 1. / this->settings.timeStepSize;

            // contribution from curve length penalization
            lhs -= delta * this->settings.mu * this->stencils.X0Y0Z0.at_grid(i, j, k);

            // compute right-handside
            double rhs = 0;

            // contribution of curve length penalization
            rhs +=
              this->settings.mu * (
                                   this->stencils.XMYMZ0.at_grid(i, j, k) * this->levelSetAccess.at_grid(i-1, j-1, k) +
                                   this->stencils.XMYPZ0.at_grid(i, j, k) * this->levelSetAccess.at_grid(i-1, j+1, k) +
                                   this->stencils.XPYMZ0.at_grid(i, j, k) * this->levelSetAccess.at_grid(i+1, j-1, k) +
                                   this->stencils.XPYPZ0.at_grid(i, j, k) * this->levelSetAccess.at_grid(i+1, j+1, k) +

                                   this->stencils.XMY0ZM.at_grid(i, j, k) * this->levelSetAccess.at_grid(i-1, j, k-1) +
                                   this->stencils.XMY0ZP.at_grid(i, j, k) * this->levelSetAccess.at_grid(i-1, j, k+1) +
                                   this->stencils.XPY0ZM.at_grid(i, j, k) * this->levelSetAccess.at_grid(i+1, j, k-1) +
                                   this->stencils.XPY0ZP.at_grid(i, j, k) * this->levelSetAccess.at_grid(i+1, j, k+1) +

                                   this->stencils.X0YMZM.at_grid(i, j, k) * this->levelSetAccess.at_grid(i, j-1, k-1) +
                                   this->stencils.X0YMZP.at_grid(i, j, k) * this->levelSetAccess.at_grid(i, j-1, k+1) +
                                   this->stencils.X0YPZM.at_grid(i, j, k) * this->levelSetAccess.at_grid(i, j+1, k-1) +
                                   this->stencils.X0YPZP.at_grid(i, j, k) * this->levelSetAccess.at_grid(i, j+1, k+1) +

                                   this->stencils.XMY0Z0.at_grid(i, j, k) * this->levelSetAccess.at_grid(i-1, j, k) +
                                   this->stencils.XPY0Z0.at_grid(i, j, k) * this->levelSetAccess.at_grid(i+1, j, k) +
                                   this->stencils.X0YMZ0.at_grid(i, j, k) * this->levelSetAccess.at_grid(i, j-1, k) +
                                   this->stencils.X0YPZ0.at_grid(i, j, k) * this->levelSetAccess.at_grid(i, j+1, k) +
                                   this->stencils.X0Y0ZM.at_grid(i, j, k) * this->levelSetAccess.at_grid(i, j, k-1) +
                                   this->stencils.X0Y0ZP.at_grid(i, j, k) * this->levelSetAccess.at_grid(i, j, k+1)

                                   );

            // contribution from area size penalization
            rhs -= this->settings.eta;

            // contribution from penalization of variance in first region
            rhs -= this->settings.lambdaRegionOne *
              pow( this->imageAccess.at_grid(i, j, k) - this->regionOneAverage, 2);

            // contribution from penalization of variance in second region
            rhs += this->settings.lambdaRegionTwo *
              pow( this->imageAccess.at_grid(i, j, k) - this->regionTwoAverage, 2);

            rhs *= delta;

            // contribution from time-dependent evolution
            rhs += copy.at_grid(i, j, k) / this->settings.timeStepSize;

            // save old value for relaxation step
            double old = this->levelSetAccess.at_grid(i, j, k);

            // perform fixed-point step
            this->levelSetAccess.at_grid(i, j, k) = this->settings.omega *
              rhs / lhs + ( 1 - this->settings.omega ) * old;

          } /* end for k */

        } /* end for j */

      } /* end for i */

    } /* end for m */

  } // end solve_current_equation_system

  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
};


#endif
