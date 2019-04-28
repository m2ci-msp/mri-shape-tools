#ifndef __EMA_INFO_H__
#define __EMA_INFO_H__

#include "ema/EmaInfoData.h"

class EmaInfo{

public:

  // constructor sets the reference to the main EmaInfoData instance
  EmaInfo(EmaInfoData& emaInfoData) : emaInfoData(emaInfoData) {
  
  }

  // return reference to vector containing all coil IDs
  const std::vector<std::string>& coil_ids() const {
  
    return this->emaInfoData.coilIds;
  
  }

  // return reference to time stamps vector
  const std::vector<double>& time_stamps() const {
  
    return this->emaInfoData.timeStamps;
  
  }

  // returns sampling frequency
  const double& sampling_frequency() const {
  
    return this->emaInfoData.samplingFrequency;
  
  }

  int sample_amount() const {
  
    return this->emaInfoData.timeStamps.size();
  }
  

private:

  // reference to the main EmaInfoData instance
  EmaInfoData& emaInfoData;

};
#endif
