#ifndef __EMA_COIL_FILTER_MEDIAN_H__
#define __EMA_COIL_FILTER_MEDIAN_H__

#include <vector>

#include <armadillo>

#include "ema/EmaCoilData.h"
#include "ema/EmaCoilAccess.h"
#include "ema/EmaCoilBoundary.h"
#include "ema/EmaCoilMirror.h"
#include "ema/EmaCoilInfo.h"

class EmaCoilFilterMedian{

public:

  EmaCoilFilterMedian(EmaCoilData& emaCoilData, const int& radius) : 
  
    emaCoilData(emaCoilData),
    emaCoilAccess(emaCoilData),
    emaCoilBoundary(emaCoilData),
    emaCoilMirror(emaCoilData),
    radius(radius) {
  
    adjust_boundaries();
  
    }

  void apply() {
  
    if( this->radius <= 0) {
  
      return;
  
    }
  
    // create copy of original data
    EmaCoilData originalData = this->emaCoilData;
    EmaCoilAccess originalAccess(originalData);
    EmaCoilInfo originalInfo(originalData);
  
    const int sampleAmount = originalInfo.sample_amount();
  
    EmaCoilMirror(originalData).positions();
    filter_positions(originalAccess, sampleAmount);
  
    if( originalInfo.euler_angles_present() ) {
  
      EmaCoilMirror(originalData).euler_angles();
      filter_euler_angles(originalAccess, sampleAmount);
  
    }
  
  }

private:

  void adjust_boundaries() {
  
    if( this->emaCoilData.boundarySize < this->radius ) {
  
      this->emaCoilBoundary.change_size(this->radius);
  
    }
  
  }

  void filter_positions(const EmaCoilAccess& access, const int& sampleAmount) {
  
    for(int i = 0; i < sampleAmount; ++i) {
  
      const arma::vec value = find_median_position(access, i);
  
      this->emaCoilAccess.position(i) = value;
  
    }
  
  }

  void filter_euler_angles(const EmaCoilAccess& access, const int& sampleAmount) {
  
    for(int i = 0; i < sampleAmount; ++i) {
  
      const arma::vec value = find_median_euler_angle(access, i);
  
      this->emaCoilAccess.euler_angle(i) = value;
  
    }
  
  }

  arma::vec find_median_position(
                                 const EmaCoilAccess& coil,
                                 const int& t
                                 ) const {
  
    std::vector<arma::vec> values;
  
    for(int i = -this->radius; i <= this->radius; ++i) {
  
      values.push_back( coil.position(t + i) );
  
    }
  
    std::sort(values.begin(), values.end(), this->compare );
  
    // compute index of center value ->
    // size is odd and indexes start at 0
    const int center = ( values.size() - 1 ) / 2;
  
    return values[center];
  
  
  }

  arma::vec find_median_euler_angle(
                                 const EmaCoilAccess& coil,
                                 const int& t
                                 ) const {
  
    std::vector<arma::vec> values;
  
    for(int i = -this->radius; i <= this->radius; ++i) {
  
      values.push_back( coil.euler_angle(t + i) );
  
    }
  
    std::sort(values.begin(), values.end(), this->compare );
  
    // compute index of center value ->
    // size is odd and indexes start at 0
    const int center = ( values.size() - 1 ) / 2;
  
    return values[center];
  
  
  }

  // reference to respective EmaCoilData
  EmaCoilData& emaCoilData;
  
  EmaCoilAccess emaCoilAccess;
  EmaCoilBoundary emaCoilBoundary;
  EmaCoilMirror emaCoilMirror;
  
  const int radius;
  
  // used for comparing two vectors
  struct Compare{
  
    bool operator() (const arma::vec& left, const arma::vec& right) {
  
      return arma::norm(left) < arma::norm(right);
  
    }
  
  } compare;

};
#endif
