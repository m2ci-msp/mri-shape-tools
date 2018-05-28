#ifndef __EMA_DATA_H__
#define __EMA_DATA_H__

#include <map>
#include <string>

#include <armadillo>

#include "ema/EmaCoil.h"

class EmaData{

public:

  // map that associates an EmaCoil object to an EMA coil id
  std::map<std::string, EmaCoil> emaCoils;

  // meta information about EMA data
  EmaInfoData emaInfoData;

};
#endif
