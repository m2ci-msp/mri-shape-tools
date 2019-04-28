#ifndef __EMA_COIL_FILTER_H__
#define __EMA_COIL_FILTER_H__

#include "ema/EmaCoilData.h"
#include "ema/EmaCoilFilterGaussian.h"
#include "ema/EmaCoilFilterMedian.h"

class EmaCoilFilter{

public:

  EmaCoilFilter(EmaCoilData& emaCoilData) : emaCoilData(emaCoilData) {
  
  }

  // apply Gaussian convolution to the positional and Euler angle data with specified standard deviation
  void gaussian(const double& stdDev) {
  
    EmaCoilFilterGaussian(this->emaCoilData, stdDev).apply();
  
  }

  // apply median filtering to the positional and Euler angle data with specified window radius
  void median(const int& radius) {
  
    EmaCoilFilterMedian(this->emaCoilData, radius).apply();
  
  }

private:

  // reference to the EmaCoilData
  EmaCoilData& emaCoilData;

};
#endif
