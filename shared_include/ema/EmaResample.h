#ifndef __EMA_RESAMPLE_H__
#define __EMA_RESAMPLE_H__

#include <vector>
#include <map>

#include <armadillo>

#include "ema/EmaData.h"

class EmaResample{

public:

  EmaResample(EmaData& emaData) : emaData(emaData) {
  
  }

  // try to resample data to specified sampling frequency
  // sets samplingFrequency in emaInfoData to achieved frequency
  void to(const double& samplingFrequency) {
  
    const int sampleAmount = compute_sample_amount(samplingFrequency);
  
    this->emaData.emaCoils = resample_coils(sampleAmount);
  
    this->emaData.emaInfoData.timeStamps = compute_time_stamps(sampleAmount);
  
    this->emaData.emaInfoData.samplingFrequency = estimate_sampling_frequency();
  
  }

private:

  // use old and new sampling frequency to determine the amount of samples to take
  int compute_sample_amount(const double& samplingFrequency) const {
  
    const int newSampleAmount =
      ( 
       // compute time of signal
       ( this->emaData.emaInfoData.timeStamps.size() - 1 ) /
       this->emaData.emaInfoData.samplingFrequency
       // multiply by new sampling frequency
        ) * samplingFrequency;
  
    return newSampleAmount;
  
  }

  // build emaCoils map for resampled data
  std::map<std::string, EmaCoil> resample_coils(const int& sampleAmount) const {
  
    std::map<std::string, EmaCoil> emaCoils;
  
    for(auto& coil:  this->emaData.emaCoils ) {
  
      const std::string& id = coil.first;
      emaCoils.emplace( id, resample_coil(id, sampleAmount) );
  
    }
  
    return emaCoils;
  
  }

  // resample coil data
  EmaCoil resample_coil(const std::string& id, const int& sampleAmount) const {
  
    // get original coil
    const EmaCoil& coil = this->emaData.emaCoils.at(id);
  
    std::vector<arma::vec> newPositions;
    std::vector<arma::vec> newEulerAngles;
  
    for(int i = 0; i <= sampleAmount - 1; ++i) {
  
      // compute time frame of sample
      const double frame = i / ( (double) sampleAmount - 1 ) * ( coil.info().sample_amount() - 1 );
  
      // add interpolated position
      newPositions.push_back(coil.interpolate().position_at_frame(frame));
  
      // add interpolated Euler angle
      if( coil.info().euler_angles_present() ) {
  
        newEulerAngles.push_back(coil.interpolate().euler_angle_at_frame(frame));
  
      }
  
    }
  
    // build resampled coil
    EmaCoilData data(this->emaData.emaInfoData);
    data.positions = newPositions;
    data.eulerAngles = newEulerAngles;
    // rms values are empty
    data.rmsValues.clear();
    data.id = id;
  
    return EmaCoil(data);
  
  }

  // recompute time stamps by using total duration and amount of samples
  std::vector<double> compute_time_stamps(const int& sampleAmount) {
  
    const double startTime = *( this->emaData.emaInfoData.timeStamps.begin() );
    const double& endTime = *( this->emaData.emaInfoData.timeStamps.end() - 1 );
  
    const double totalDuration = endTime - startTime;
  
    std::vector<double> newTimeStamps;
  
    for(int i = 0; i <= sampleAmount - 1; ++i) {
  
      // compute time stamp
      const double timeStamp = startTime + i / ( (double) sampleAmount - 1 ) * ( totalDuration );
  
      // save time stamp
      newTimeStamps.push_back(timeStamp);
  
    }
  
    return newTimeStamps;
  
  }

  double estimate_sampling_frequency() const {
  
      const double& startTime = *( this->emaData.emaInfoData.timeStamps.begin() );
      const double& endTime = *( this->emaData.emaInfoData.timeStamps.end() - 1);
  
      const double totalDuration = endTime - startTime;
  
      return this->emaData.emaInfoData.timeStamps.size() / totalDuration;
  
  }

  // reference to main EmaData instance
  EmaData& emaData;

};
#endif
