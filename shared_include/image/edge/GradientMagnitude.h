#ifndef __LAPLACIAN_ZERO_CROSSINGS_H__
#define __LAPLACIAN_ZERO_CROSSINGS_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageBoundary.h"
#include "image/ImageCalculus.h"
#include "image/ImageMirror.h"

class LaplacianZeroCrossings{

private:

  ImageData& imageData;

public:

  LaplacianZeroCrossings(ImageData& imageData) :

    imageData(imageData) {

  }

  void apply() {

    compute_laplacian();
    compute_zero_crossings();

  }

private:

  void compute_laplacian() {

    ImageData copy = this->imageData;
    ImageBoundary(copy).change(1, 1, 1);
    ImageMirror(copy).all();

    ImageCalculus calculus(copy);
    ImageAccess access(this->imageData);

    for(int i = 0; i < copy.nx; ++i) {
      for(int j = 0; j < copy.ny; ++j) {
        for(int k = 0; k < copy.nz; ++k) {

          access.at_grid(i, j, k) = calculus.laplacian(i, j, k);

        }
      }

    }

  }

  void compute_zero_crossings() {

    ImageData copy = this->imageData;
    ImageBoundary(copy).change(1, 1, 1);
    ImageMirror(copy).all();

    ImageAccess accessCopy(copy);
    ImageAccess accessOriginal(this->imageData);

    for(int i = 0; i < copy.nx; ++i) {
      for(int j = 0; j < copy.ny; ++j) {
        for(int k = 0; k < copy.nz; ++k) {

          int minus = 0;
          int plus = 0;
          int zero = 0;

          for(int x = -1; x <= 1; ++x) {
            for(int y = -1; y <= 1; ++y) {
              for(int z = -1; z <= 1; ++z) {

                const double& value = accessCopy.at_grid(i + x, j + y, k + z);

                if( value == 0 ) { ++zero; }
                if( value <  0 ) { ++minus; }
                if( value >  0 ) { ++plus; }


              }
            }
          }

          accessOriginal.at_grid(i, j, k) = ( ( minus > 0 && plus > 0 ) || zero > 0)? 255: 0;

        }
      }
    }

  }

};
#endif
