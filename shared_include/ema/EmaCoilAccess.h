#ifndef __EMA_COIL_ACCESS_H__
#define __EMA_COIL_ACCESS_H__

#include <armadillo>
#include <stdexcept>

#include "ema/EmaCoilData.h"

class EmaCoilAccess{

public:

  EmaCoilAccess(EmaCoilData& emaCoilData) : emaCoilData(emaCoilData) {
  }

  arma::vec& position(const int& index) {
  
    // consider boundary
    const int& boundarySize = this->emaCoilData.boundarySize;
  
    return this->emaCoilData.positions.at(boundarySize + index);
  
  }
  
  // const version
  const arma::vec& position(const int& index) const {
  
    // consider boundary
    const int& boundarySize = this->emaCoilData.boundarySize;
  
    return this->emaCoilData.positions.at(boundarySize + index);
  
  }

  arma::vec& euler_angle(const int& index) {
  
    // consider boundary
    const int& boundarySize = this->emaCoilData.boundarySize;
  
    return this->emaCoilData.eulerAngles.at(boundarySize + index);
  
  }
  
  // const version
  const arma::vec& euler_angle(const int& index) const {
  
    // consider boundary
    const int& boundarySize = this->emaCoilData.boundarySize;
  
    return this->emaCoilData.eulerAngles.at(boundarySize + index);
  
  }

  double& rms_value(const int& index) {
  
    // do not consider boundary in this case
    return this->emaCoilData.rmsValues.at(index);
  
  }
  
  // const version
  const double& rms_value(const int& index) const {
  
    // do not consider boundary in this case
    return this->emaCoilData.rmsValues.at(index);
  
  }

private:

  // reference to the data that should be accessed
  EmaCoilData& emaCoilData;

};
#endif
