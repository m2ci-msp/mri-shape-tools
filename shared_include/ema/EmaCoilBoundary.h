#ifndef __EMA_COIL_BOUNDARY_H__
#define __EMA_COIL_BOUNDARY_H__

#include "ema/EmaCoilAccess.h"
#include "ema/EmaCoilData.h"
#include "ema/EmaCoilInfo.h"

class EmaCoilBoundary{

public:

  EmaCoilBoundary(EmaCoilData& emaCoilData) : 
  
    emaCoilData(emaCoilData),
    emaCoilAccess(emaCoilData) {
  
    }

  void change_size(const int& size) {
  
    // create copy of original data
    EmaCoilData originalData = this->emaCoilData;
  
    EmaCoilAccess originalAccess(originalData);
  
    EmaCoilInfo originalInfo(originalData);
  
    const int sampleAmount = this->emaCoilData.emaInfoData.get().timeStamps.size();
  
    // update boundary size
    this->emaCoilData.boundarySize = size;
  
    // resize data storages
    this->emaCoilData.positions.resize(sampleAmount + 2 * size);
  
    if( originalInfo.euler_angles_present() ) {
  
      this->emaCoilData.eulerAngles.resize(sampleAmount + 2 * size);
  
    }
  
    // copy original data
    for(int i = 0; i < sampleAmount; ++i) {
  
      this->emaCoilAccess.position(i) = originalAccess.position(i);
  
    }
  
    if( originalInfo.euler_angles_present() ) {
  
      for(int i = 0; i < sampleAmount; ++i) {
  
        this->emaCoilAccess.euler_angle(i) = originalAccess.euler_angle(i);
  
      }
  
    }
  
  }

private:

  // reference to the EmaCoilData instance
  EmaCoilData& emaCoilData;
  
  EmaCoilAccess emaCoilAccess;

};
#endif
