#ifndef __EMA_COIL_MIRROR_H__
#define __EMA_COIL_MIRROR_H__

#include "ema/EmaCoilData.h"
#include "ema/EmaCoilAccess.h"

class EmaCoilMirror{

public:

  EmaCoilMirror(EmaCoilData& emaCoilData) : 
  
    emaCoilData(emaCoilData),
    emaCoilAccess(emaCoilData) {
  
    }

  void positions() {
  
    const int sampleAmount = this->emaCoilData.emaInfoData.timeStamps.size();
    const int& boundarySize = this->emaCoilData.boundarySize;
  
    for(int i = 0; i < boundarySize; ++i) {
  
      // left boundary
      this->emaCoilAccess.position(- i - 1) = this->emaCoilAccess.position(i);
  
      // right boundary
      this->emaCoilAccess.position(sampleAmount + i) = this->emaCoilAccess.position(sampleAmount - 1 - i);
  
    }
  
  }

  void euler_angles() {
  
      const int sampleAmount = this->emaCoilData.emaInfoData.timeStamps.size();
      const int& boundarySize = this->emaCoilData.boundarySize;
  
      for(int i = 0; i < boundarySize; ++i) {
  
        // left boundary
        this->emaCoilAccess.euler_angle(- i - 1) = this->emaCoilAccess.euler_angle(i);
  
        // right boundary
        this->emaCoilAccess.euler_angle(sampleAmount + i) = this->emaCoilAccess.euler_angle(sampleAmount - 1 - i);
  
      }
  
    }

private:

  EmaCoilData& emaCoilData;
  
  EmaCoilAccess emaCoilAccess;

};
#endif
