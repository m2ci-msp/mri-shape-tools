#ifndef __IMAGE_EDGE_H__
#define __IMAGE_EDGE_H__

#include "image/ImageData.h"
#include "image/edge/LaplacianZeroCrossings.h"
#include "image/edge/GradientMagnitude.h"
#include "image/edge/LargestEigenvalueStructureTensor.h"

class ImageEdge{

public:

  ImageEdge(ImageData& imageData) : imageData(imageData) {
  }

  void laplacian_zero_crossings() {

      LaplacianZeroCrossings(this->imageData).apply();

  }

  void gradient_magnitude() {

    GradientMagnitude(this->imageData).apply();

  }

  void largest_eigenvalue_structure_tensor(const double& radius) {

    LargestEigenvalueStructureTensor(this->imageData, radius).apply();

  }

  private:

  ImageData& imageData;

};

#endif
