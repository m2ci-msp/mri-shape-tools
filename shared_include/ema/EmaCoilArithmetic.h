#ifndef __EMA_COIL_ARITHMETIC_H__
#define __EMA_COIL_ARITHMETIC_H__

#include "ema/EmaCoilData.h"
#include "ema/EmaCoilAccess.h"

class EmaCoilArithmetic{

public:

  EmaCoilArithmetic(EmaCoilData& emaCoilData) : 
  
  emaCoilData(emaCoilData),
  emaCoilAccess(emaCoilData) {
  
  }

  void plus(const EmaCoilAccess& other) {
  
    const int sampleAmount = this->emaCoilData.emaInfoData.get().timeStamps.size();
  
    for(int i = 0; i < sampleAmount; ++i) {
  
      this->emaCoilAccess.position(i) += other.position(i);
  
    }
  
    // invalidate Euler angles and RMS values
    this->emaCoilData.eulerAngles.clear();
  
    this->emaCoilData.rmsValues.clear();
  
  }
  void minus(const EmaCoilAccess& other) {
  
    const int sampleAmount = this->emaCoilData.emaInfoData.get().timeStamps.size();
  
    for(int i = 0; i < sampleAmount; ++i) {
  
      this->emaCoilAccess.position(i) -= other.position(i);
  
    }
  
    // invalidate Euler angles and RMS values
    this->emaCoilData.eulerAngles.clear();
  
    this->emaCoilData.rmsValues.clear();
  
  }

  

private:

  // reference to corresponding EmaCoilData instance
  EmaCoilData& emaCoilData;
  
  EmaCoilAccess emaCoilAccess;


};
#endif
