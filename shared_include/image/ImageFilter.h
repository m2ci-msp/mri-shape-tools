#ifndef __IMAGE_FILTER_H__
#define __IMAGE_FILTER_H__

#include "image/ImageData.h"
#include "image/filter/diffusion/DiffusionSettings.h"

#include "image/filter/GaussianFilter.h"
#include "image/filter/DiffusionFilter.h"
#include "image/filter/MedianFilter.h"

class ImageFilter{

public:

  ImageFilter(ImageData& imageData) : imageData(imageData) {
  }

  void gaussian(
                const double& stdDev
                ) {
  
    GaussianFilter filter(this->imageData, stdDev);
    filter.apply();
  
  }

  void median(
                const int& radius
                ) {
  
    MedianFilter filter(this->imageData, radius);
    filter.apply();
  
  }

  void diffusion(
                 const int& timeSteps,
                 const double& stepSize,
                 const double& contrastLambda,
                 const double& integrationRho,
                 const double& presmoothSigma
                 ) {
  
    DiffusionSettings settings;
    settings.timeSteps = timeSteps;
    settings.stepSize = stepSize;
    settings.contrastLambda = contrastLambda;
    settings.integrationRho = integrationRho;
    settings.presmoothSigma = presmoothSigma;
  
    DiffusionFilter filter(this->imageData, settings);
    filter.apply();
  
  }

private:

  ImageData& imageData;

};

#endif
