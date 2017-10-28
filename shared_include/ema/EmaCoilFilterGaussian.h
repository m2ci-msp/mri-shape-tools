#ifndef __EMA_COIL_FILTER_GAUSSIAN_H__
#define __EMA_COIL_FILTER_GAUSSIAN_H__

#include <vector>

#include <armadillo>

#include "gaussian/GaussianBuilder.h"

#include "ema/EmaCoilData.h"
#include "ema/EmaCoilAccess.h"
#include "ema/EmaCoilBoundary.h"
#include "ema/EmaCoilMirror.h"
#include "ema/EmaCoilInfo.h"

class EmaCoilFilterGaussian{

public:

  EmaCoilFilterGaussian(EmaCoilData& emaCoilData, const double& stdDev) : 
  
    emaCoilData(emaCoilData),
    emaCoilAccess(emaCoilData),
    emaCoilBoundary(emaCoilData),
    emaCoilMirror(emaCoilData),
    stdDev(stdDev) {
  
    init_gauss_kernel();
  
    adjust_boundaries();
  
    }

  void apply() {
  
    if( this->stdDev <= 0) {
  
      return;
  
    }
  
    // create copy of original data
    EmaCoilData originalData = this->emaCoilData;
    EmaCoilAccess originalAccess(originalData);
    EmaCoilInfo originalInfo(originalData);
  
    const int sampleAmount = originalInfo.sample_amount();
  
    EmaCoilMirror(originalData).positions();
    convolve_positions(originalAccess, sampleAmount);
  
    if( originalInfo.euler_angles_present() ) {
  
      EmaCoilMirror(originalData).euler_angles();
      convolve_euler_angles(originalAccess, sampleAmount);
  
    }
  
  }

private:

  void init_gauss_kernel() {
  
    // radius of truncated gaussian to each side in terms
    // of standard deviation
    // we use a radius of 3 stdDev
    const int size = 3;
  
    this->kernel = GaussianBuilder::build(this->stdDev, size);
  
    this->radius = (this->kernel.size() - 1) / 2;
  
  }

  void adjust_boundaries() {
  
    if( this->emaCoilData.boundarySize < this->radius ) {
  
      this->emaCoilBoundary.change_size(this->radius);
  
    }
  
  }

  void convolve_positions(const EmaCoilAccess& access, const int& sampleAmount) {
  
    for(int i = 0; i < sampleAmount; ++i) {
  
      arma::vec value = arma::zeros(3);
  
      for( int r = -this->radius; r <= this->radius; ++r) {
  
        value += this->kernel[this->radius + r] * access.position(i + r);
  
      }
  
      this->emaCoilAccess.position(i) = value;
  
    }
  
  }

  void convolve_euler_angles(const EmaCoilAccess& access, const int& sampleAmount) {
  
    for(int i = 0; i < sampleAmount; ++i) {
  
      arma::vec value = arma::zeros(2);
  
      for( int r = -this->radius; r <= this->radius; ++r) {
  
        value += this->kernel[this->radius + r] * access.euler_angle(i + r);
  
      }
  
      this->emaCoilAccess.euler_angle(i) = value;
  
    }
  
  }

  // reference to respective EmaCoilData instance
  EmaCoilData& emaCoilData;
  
  EmaCoilAccess emaCoilAccess;
  EmaCoilBoundary emaCoilBoundary;
  EmaCoilMirror emaCoilMirror;
  
  const double stdDev;
  
  int radius;
  
  std::vector<double> kernel;

};
#endif
