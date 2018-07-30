#ifndef __OTSU_ENERGY_H__
#define __OTSU_ENERGY_H__

#include <cmath>

#include "image/ImageData.h"

class OtsuEnergy{

private:

  const ImageData& imageData;

  ImageAccess imageAccess;

  std::vector<double> histogram;

  std::vector<double> zeroMoments;

  std::vector<double> means;

  double meanTotal;

public:

  // IMPORTANT: colors have to be in the range [0, 255]
  OtsuEnergy(ImageData& imageData) : imageData(imageData), imageAccess(imageData) {

    compute_histogram();

    compute_zero_order_cumulative_moments();

    compute_means();

    compute_mean_total();

  }

  double get_energy(const double& threshold) {

    return

      pow(this->meanTotal * this->zeroMoments.at(threshold) - this->means.at(threshold), 2) /
      this->zeroMoments.at(threshold) * ( 1. - this->zeroMoments.at(threshold));

  }

private:

  void compute_mean_total() {

    this->meanTotal = 0;

    for(int i = 0; i < 256; ++i) {

      this->meanTotal += i * this->histogram.at(i);

    }

  }

  void compute_zero_order_cumulative_moments() {

    this->zeroMoments.resize(256, 0);

    for(int k = 0; k < 256; ++k) {

      double moment = 0;

      for(int i = 0; i <= k; ++i) {

        moment += this->histogram.at(i);

      }

      this->zeroMoments.at(k) = moment;

    }



  }

  void compute_histogram() {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    this->histogram.resize(256, 0);

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for( int k = 0; k < nz; ++k) {

          const double value = this->imageAccess.at_grid(i, j, k);
          this->histogram[(int) value] += 1;

        }

      }

    }

    const int size = nx * ny * nz;

    for(double& value: this->histogram) {

      value /= size;

    }

  }

  void compute_means() {

    this->means.resize(256, 0);

    for(int k = 0; k < 256; ++k) {

      double mean = 0;

      for(int i = 0; i <= k; ++i) {

        mean += i * this->histogram.at(i);

      }

      this->means.at(k) = mean;

    }

  }

};
#endif
