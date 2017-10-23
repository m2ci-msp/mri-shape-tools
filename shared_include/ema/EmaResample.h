#ifndef __EMA_RESAMPLE_H__
#define __EMA_RESAMPLE_H__

#include <vector>
#include <armadillo>

#include "ema/EmaData.h"
#include "ema/EmaInterpolateCoil.h"

class EmaResample{

public:

  EmaResample(EmaData& emaData) : emaData(emaData) {
  
  }

  // try to resample data to specified sampling frequency
  // sets samplingFrequency in emaData to achieved frequency
  void to(const double& samplingFrequency) {
  
    const int sampleAmount = compute_sample_amount(samplingFrequency);
  
    for(auto& coil:  this->emaData.emaCoils ) {
  
      resample_positions(coil.second.position(), sampleAmount);
  
    }
  
    recompute_time_stamps(sampleAmount);
  
    this->emaData.samplingFrequency = estimate_sampling_frequency();
  
  }

private:

  // use old and new sampling frequency to determine the amount of samples to take
  int compute_sample_amount(const double& samplingFrequency) const {
  
    const int newSamplingFrequency =
      ( this->emaData.timeStamps.size() / this->emaData.samplingFrequency ) * samplingFrequency;
  
    return newSamplingFrequency;
  
  }

  // resample positional data
  void resample_positions(std::vector<arma::vec>& coilPositions, const int& sampleAmount) {
  
    EmaInterpolateCoil interpolate(coilPositions, this->emaData.timeStamps);
    std::vector<arma::vec> newPositions;
  
    for(int i = 0; i <= sampleAmount - 1; ++i) {
  
      // compute time frame of sample
      const double frame = i / ( (double) sampleAmount - 1 ) * ( coilPositions.size() - 1 );
  
      // add interpolated position
      newPositions.push_back(interpolate.at_frame(frame));
  
    }
  
    // set new values
    coilPositions.clear();
    coilPositions.insert(coilPositions.begin(), newPositions.begin(), newPositions.end());
  
  }

  // recompute time stamps by using total duration and amount of samples
  void recompute_time_stamps(const int& sampleAmount) {
  
    const double startTime = *( this->emaData.timeStamps.begin() );
    const double& endTime = *( this->emaData.timeStamps.end() - 1 );
  
    const double totalDuration = endTime - startTime;
  
    this->emaData.timeStamps.clear();
  
    for(int i = 0; i <= sampleAmount - 1; ++i) {
  
      // compute time stamp
      const double timeStamp = startTime + i / ( (double) sampleAmount - 1 ) * ( totalDuration );
  
      // save time stamp
      this->emaData.timeStamps.push_back(timeStamp);
  
    }
  
  }

  double estimate_sampling_frequency() const {
  
      const double& startTime = *( this->emaData.timeStamps.begin() );
      const double& endTime = *( this->emaData.timeStamps.end() - 1);
  
      const double totalDuration = endTime - startTime;
  
      return this->emaData.timeStamps.size() / totalDuration;
  
  }

  // reference to main EmaData instance
  EmaData& emaData;

};
#endif
