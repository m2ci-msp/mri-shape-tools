#ifndef __EMA_COIL_INFO_H__
#define __EMA_COIL_INFO_H__

#include "ema/EmaCoilData.h"

class EmaCoilInfo{

public:

  EmaCoilInfo(const EmaCoilData& emaCoilData) : emaCoilData(emaCoilData) {
  
  }

  int sample_amount() const {
  
    return this->emaCoilData.emaInfoData.timeStamps.size();
  
  }

  bool positions_present() const {
  
    return (
            ( this->emaCoilData.positions.size() - 2 * this->emaCoilData.boundarySize ) ==
              this->emaCoilData.emaInfoData.timeStamps.size() 
            );
  
  }

  bool euler_angles_present() const {
  
    return (
            ( this->emaCoilData.eulerAngles.size() - 2 * this->emaCoilData.boundarySize ) ==
              this->emaCoilData.emaInfoData.timeStamps.size() 
            );
  
  }

  bool rms_values_present() const {
  
    return this->emaCoilData.rmsValues.size() == this->emaCoilData.emaInfoData.timeStamps.size();
  
  }

private:

  // reference to corresponding EmaCoilData instance
  const EmaCoilData& emaCoilData;

};
#endif
