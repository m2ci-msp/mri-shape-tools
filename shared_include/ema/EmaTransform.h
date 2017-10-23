#ifndef __EMA_TRANSFORM_H__
#define __EMA_TRANSFORM_H__

#include "ema/EmaTransformCoil.h"
#include "ema/EmaData.h"

class EmaTransform{

public:

  EmaTransform(EmaData& emaData) : emaData(emaData) {
  
  }

  EmaTransformCoil coil(const std::string& coilId) {
  
    return EmaTransformCoil(this->emaData.emaCoils.at(coilId).position());
  
  }

private:

  EmaData& emaData;

};
#endif
