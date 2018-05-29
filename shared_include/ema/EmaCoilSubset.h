#ifndef __EMA_COIL_SUBSET_H__
#define __EMA_COIL_SUBSET_H__

#include <algorithm>
#include <string>

#include "ema/EmaData.h"
#include "ema/EmaCoil.h"

class EmaCoilSubset{

private:

  EmaData& emaData;

public:

  EmaCoilSubset(EmaData& emaData) : emaData(emaData) {

  }

  void to(const std::vector<std::string>& coilIds) {

    const std::vector<std::string>& originalIds = this->emaData.emaInfoData.coilIds;

    for(const std::string& id: originalIds) {

      if( std::find(coilIds.begin(), coilIds.end(), id) == coilIds.end() ) {

        this->emaData.emaCoils.erase(id);

      }

    }

    this->emaData.emaInfoData.coilIds = coilIds;

  }

};
#endif
