#ifndef __WEIGHTED_OTSU_ENERGY_H__
#define __WEIGHTED_OTSU_ENERGY_H__

#include <cmath>

#include "image/ImageData.h"
#include "image/segmentation/Threshold.h"

class WeightedOtsuEnergy{

private:

  const ImageData& imageData;

  ImageData segmentation;

  double meanInside;
  double meanOutside;

  double weightInside = 1;
  double weightOutside = 1;

public:

  WeightedOtsuEnergy(const ImageData& imageData) : imageData(imageData) {
  }

  double get_energy(const double& threshold) {

    // compute segmentation
    this->segmentation = this->imageData;
    Threshold(this->segmentation, threshold).apply();

    // compute region means
    compute_means();

    // compute sum of variances
    const double energy = compute_variance_sum();

    return energy;

  }

  void set_weights(
                          const double& weightInside,
                          const double& weightOutside
                          ) {

    this->weightInside = weightInside;
    this->weightOutside = weightOutside;

  }

private:

  void compute_means() {

    this->meanOutside = 0;
    this->meanInside = 0;

    const auto& indicator = this->segmentation.values;
    const auto& values = this->imageData.values;

    int amountOutside = 0;
    int amountInside = 0;

    for(unsigned int i = 0; i < values.size(); ++i) {
      if( indicator.at(i) == 0 ) {
        ++amountOutside;
        this->meanOutside += values.at(i);

      }
      else {
        ++amountInside;
        this->meanInside += values.at(i);
      }
    }

    this->meanOutside /= amountOutside;
    this->meanInside /= amountInside;

  }

  double compute_variance_sum() {

    const auto& indicator = this->segmentation.values;
    const auto& values = this->imageData.values;

    double varOutside = 0;
    double varInside = 0;
    int amountOutside = 0;
    int amountInside = 0;

    // compute variances inside the regions
    for(unsigned int i = 0; i < values.size(); ++i) {
      if( indicator.at(i) == 0 ) {

        const double dist = values.at(i) - this->meanOutside;

        ++amountOutside;
        varOutside += pow(dist, 2);

      }
      else {

        const double dist = values.at(i) - this->meanInside;

        ++amountInside;
        varInside += pow(dist, 2);

      }
    }

    const double varianceSum =
      this->weightOutside * varOutside / amountOutside +
      this->weightInside * varInside / amountInside;

    return  varianceSum;

  }

};
#endif
