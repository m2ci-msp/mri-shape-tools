#ifndef __EMA_ACCESS_H__
#define __EMA_ACCESS_H__

#include "ema/EmaData.h"

class EmaAccess{

public:

  // constructor sets the reference to the main EmaData instance
  EmaAccess(EmaData& emaData) : emaData(emaData) {
  
  }

  // return reference to EmaCoil object for given ID
  EmaCoil& coil(const std::string& id) {
  
    return this->emaData.emaCoils.at(id);
  
  }

  // return reference to vector containing all coil IDs
  const std::vector<std::string>& coil_ids() const {
  
    return this->emaData.emaCoilIds;
  
  }

  // return reference to time stamps vector
  const std::vector<double>& time_stamps() const {
  
    return this->emaData.timeStamps;
  
  }

  // returns sampling frequency
  const double& sampling_frequency() const {
  
    return this->emaData.samplingFrequency;
  
  }

private:

  // reference to the main EmaData instance
  EmaData& emaData;

};
#endif
