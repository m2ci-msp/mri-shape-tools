#ifndef __EMA_COIL_DATA_H__
#define __EMA_COIL_DATA_H__

#include <vector>
#include <string>
#include <functional>

#include <armadillo>

#include "ema/EmaInfoData.h"

class EmaCoilData{

public:

  // sets reference to the meta information
  EmaCoilData(const EmaInfoData& emaInfoData) :
  
    emaInfoData(std::cref(emaInfoData)) {
  
  }

  // coil positions
  std::vector<arma::vec> positions;
  
  // Euler angles of the coil
  std::vector<arma::vec> eulerAngles;
  
  // rms values of the coil
  std::vector<double> rmsValues;
  
  // boundary size
  int boundarySize = 0;
  
  // id of the coil
  std::string id;
  
  // reference to main EmaInfoData instance
  std::reference_wrapper<const EmaInfoData> emaInfoData;

};
#endif
