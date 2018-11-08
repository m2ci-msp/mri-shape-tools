#ifndef __EMA_BUILDER_H__
#define __EMA_BUILDER_H__

#include "ema/EmaData.h"

class EmaBuilder{

public:

  // constructor sets the reference to the main EmaData instance
  EmaBuilder(EmaData& emaData) : emaData(emaData) {

  }

  void from(const EmaData& emaData) {

    this->emaData.emaInfoData = emaData.emaInfoData;
    this->emaData.emaCoils = emaData.emaCoils;

  }

private:

  // reference to main EmaData instance
  EmaData& emaData;

};
#endif
