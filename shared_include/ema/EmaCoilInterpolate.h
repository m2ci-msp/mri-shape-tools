#ifndef __EMA_COIL_INTERPOLATE_H__
#define __EMA_COIL_INTERPOLATE_H__

#include <vector>
#include <armadillo>

#include "ema/EmaData.h"
#include "ema/EmaCoilAccess.h"

class EmaCoilInterpolate{

public:

  EmaCoilInterpolate(EmaCoilData& emaCoilData) : 

    emaCoilData(emaCoilData),
    emaCoilAccess(emaCoilData) {
  
    }

  // interpolate position at given time stamp
  arma::vec position_at_time(const double& timeStamp) const {
  
    const double timeFrame = convert_to_frame(timeStamp);
  
    return position_at_frame(timeFrame);
  
  }

  // interpolate position at given time frame
  arma::vec position_at_frame(const double& timeFrame) const {
  
    const int integerPart = (int) timeFrame;
    const double subIntegerPart = timeFrame - integerPart;
  
    // get values at participating frames
    const arma::vec& left = this->emaCoilAccess.position(integerPart);
  
    // skip interpolation if sub integer part is 0
    if( subIntegerPart == 0 ) {
  
      return left;
  
    }
  
    const arma::vec& right = this->emaCoilAccess.position(integerPart + 1);
  
    // interpolate
    return left + subIntegerPart * ( right - left );
  
  }

  // interpolate Euler angle at given time stamp
  arma::vec euler_angle_at_time(const double& timeStamp) const {
  
    const double timeFrame = convert_to_frame(timeStamp);
  
    return euler_angle_at_frame(timeFrame);
  
  }

  // interpolate Euler angle at given time frame
  arma::vec euler_angle_at_frame(const double& timeFrame) const {
  
    const int integerPart = (int) timeFrame;
    const double subIntegerPart = timeFrame - integerPart;
  
    // get values at participating frames
    const arma::vec& left = this->emaCoilAccess.euler_angle(integerPart);
  
    // skip interpolation if sub integer part is 0
    if( subIntegerPart == 0 ) {
  
      return left;
  
    }
  
    const arma::vec& right = this->emaCoilAccess.euler_angle(integerPart + 1);
  
    // interpolate
    return left + subIntegerPart * ( right - left );
  
  }

private:

  double convert_to_frame(const double& timeStamp) const {
  
    const double& startTime = *( this->emaCoilData.emaInfoData.timeStamps.begin() );
    const double& endTime = *( this->emaCoilData.emaInfoData.timeStamps.end() - 1 );
  
    const double totalDuration = endTime - startTime;
  
    // map to interval [0, 1]
    const double normalizedFrame = ( timeStamp - startTime ) / totalDuration;
  
    // rescale, make sure that last index is timeStamps.size() - 1
    return normalizedFrame * ( this->emaCoilData.emaInfoData.timeStamps.size() - 1 );
  
  }

  // reference to the EmaCoilData object
  EmaCoilData& emaCoilData;
  
  const EmaCoilAccess emaCoilAccess;

};
#endif
