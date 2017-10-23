#ifndef __EMA_INTERPOLATE_H__
#define __EMA_INTERPOLATE_H__

#include <vector>
#include <armadillo>

#include "ema/EmaInterpolateCoil.h"
#include "ema/EmaData.h"

class EmaInterpolate{

public:

  EmaInterpolate(EmaData& emaData) : emaData(emaData) {
  
  }

  EmaInterpolateCoil coil(const std::string& coilId) {
  
    return EmaInterpolateCoil(
                              this->emaData.emaCoils.at(coilId).position(),
                              this->emaData.timeStamps
                              );
  
  }

private:

  EmaData& emaData;

};
#endif
