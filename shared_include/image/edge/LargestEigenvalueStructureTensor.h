#ifndef __LARGEST_EIGENVALUE_STRUCTURE_TENSOR_H__
#define __LARGEST_EIGENVALUE_STRUCTURE_TENSOR_H__

#include <armadillo>

#include "image/ImageData.h"
#include "image/ImageAccess.h"
#include "image/ImageBoundary.h"
#include "image/ImageCalculus.h"
#include "image/ImageMirror.h"

class LargestEigenvalueStructureTensor{

private:

  ImageData& imageData;
  const double radius;

public:

  LargestEigenvalueStructureTensor(ImageData& imageData, const double& radius) :

    imageData(imageData),
    radius(radius) {

  }

  void apply() {

    compute_magnitude();

  }

private:

  void compute_magnitude() {

    ImageData copy = this->imageData;
    ImageBoundary(copy).change(this->radius + 1, this->radius + 1, this->radius + 1);
    ImageMirror(copy).all();

    ImageCalculus calculus(copy);
    ImageAccess access(this->imageData);

    for(int i = 0; i < copy.nx; ++i) {
      for(int j = 0; j < copy.ny; ++j) {
        for(int k = 0; k < copy.nz; ++k) {

          const arma::mat structureTensor = calculus.region_structure_tensor(i, j, k, radius);
          arma::vec eigval;
          arma::mat eigvec;

          arma::eig_sym(eigval, eigvec, structureTensor, "std");

          access.at_grid(i, j, k) = eigval(2);

        }
      }
    }

  }

};
#endif
