#ifndef __EMA_COIL_CALCULUS_H__
#define __EMA_COIL_CALCULUS_H__

#include "ema/EmaCoilData.h"
#include "ema/EmaCoilAccess.h"

class EmaCoilCalculus{

public:

  EmaCoilCalculus(EmaCoilData& emaCoilData) :
  
    emaCoilData(emaCoilData),
    emaCoilAccess(emaCoilData) {
  
    }

  // compute first order derivative of positional data with respect to time by using forward differences
  arma::vec position_t(const int& index) const {
  
    // compute grid size
    const double h = 1 / this->emaCoilData.emaInfoData.samplingFrequency;
  
    const arma::vec& left = this->emaCoilAccess.position(index);
  
    const arma::vec& right = this->emaCoilAccess.position(index + 1);
  
    return (right - left) / h;
  
  }

  // compute second order derivative of positional data with respect to time
  arma::vec position_tt(const int& index) {
  
      // compute grid size
      const double h = 1 / this->emaCoilData.emaInfoData.samplingFrequency;
  
      const arma::vec& left   = this->emaCoilAccess.position(index - 1);
      const arma::vec& center = this->emaCoilAccess.position(index    );
      const arma::vec& right  = this->emaCoilAccess.position(index + 1);
  
      return ( left + 2 * center + right ) / ( pow( h, 2 ) );
  
  }

  // compute first order derivative of Euler angle data with respect to time by using forward differences
  arma::vec euler_angle_t(const int& index) const {
  
    // compute grid size
    const double h = 1 / this->emaCoilData.emaInfoData.samplingFrequency;
  
    const arma::vec& left = this->emaCoilAccess.euler_angle(index);
  
    const arma::vec& right = this->emaCoilAccess.euler_angle(index + 1);
  
    return (right - left) / h;
  
  }

  // compute second order derivative of positional data with respect to time
  arma::vec euler_angle_tt(const int& index) {
  
      // compute grid size
      const double h = 1 / this->emaCoilData.emaInfoData.samplingFrequency;
  
      const arma::vec& left   = this->emaCoilAccess.euler_angle(index - 1);
      const arma::vec& center = this->emaCoilAccess.euler_angle(index    );
      const arma::vec& right  = this->emaCoilAccess.euler_angle(index + 1);
  
      return ( left + 2 * center + right ) / ( pow( h, 2 ) );
  
  }

private:

  // reference to EmaCoilData instance
  EmaCoilData& emaCoilData;
  
  EmaCoilAccess emaCoilAccess;

};
#endif
