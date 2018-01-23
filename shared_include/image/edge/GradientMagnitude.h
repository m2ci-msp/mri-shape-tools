#ifndef __GRADIENT_MAGNITUDE_H__
#define __GRADIENT_MAGNITUDE_H__

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageBoundary.h"
#include "image/ImageCalculus.h"
#include "image/ImageMirror.h"

class GradientMagnitude{

private:

  ImageData& imageData;

public:

  GradientMagnitude(ImageData& imageData) :

    imageData(imageData) {

  }

  void apply() {

    compute_gradient_magnitude();

  }

private:

  void compute_gradient_magnitude() {

    ImageData copy = this->imageData;
    ImageBoundary(copy).change(1, 1, 1);
    ImageMirror(copy).all();

    ImageCalculus calculus(copy);
    ImageAccess access(this->imageData);

    for(int i = 0; i < copy.nx; ++i) {
      for(int j = 0; j < copy.ny; ++j) {
        for(int k = 0; k < copy.nz; ++k) {

          access.at_grid(i, j, k) = arma::norm(calculus.gradient(i, j, k));

        }
      }
    }

  }

};
#endif
