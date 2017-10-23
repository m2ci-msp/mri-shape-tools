#ifndef __EMA_INTERPOLATE_COIL_H__
#define __EMA_INTERPOLATE_COIL_H__

#include <vector>
#include <armadillo>

class EmaInterpolateCoil{

public:

  EmaInterpolateCoil(
                     const std::vector<arma::vec>& coilPositions,
                     const std::vector<double>& timeStamps
                     ) : 
    coilPositions(coilPositions),
    timeStamps(timeStamps) {
  
    }

  // interpolate value at given time stamp
  arma::vec at_time(const double& timeStamp) const {
  
    const double timeFrame = convert_to_frame(timeStamp);
  
    return at_frame(timeFrame);
  
  }

  // interpolate at given time frame
  arma::vec at_frame(const double& timeFrame) const {
  
    const int integerPart = (int) timeFrame;
    const double subIntegerPart = timeFrame - integerPart;
  
    // get values at participating frames
    const arma::vec& left = this->coilPositions.at(integerPart);
  
    // skip interpolation if sub integer part is 0
    if( subIntegerPart == 0 ) {
  
    return left;
  
    }
  
    const arma::vec& right = this->coilPositions.at(integerPart + 1);
  
    // interpolate
    return left + subIntegerPart * ( right - left );
  
  }

private:

  double convert_to_frame(const double& timeStamp) const {
  
    const double& startTime = *( this->timeStamps.begin() );
    const double& endTime = *( this->timeStamps.end() - 1 );
  
    const double totalDuration = endTime - startTime;
  
    // map to interval [0, 1]
    const double normalizedFrame = ( timeStamp - startTime ) / totalDuration;
  
    // rescale 
    return normalizedFrame * this->coilPositions.size();
  
  }

  // reference to positions of corresponding coil
  const std::vector<arma::vec>& coilPositions;
  // time stamps of the individual time frames
  const std::vector<double>& timeStamps;

};
#endif
