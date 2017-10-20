#ifndef __EMA_DATA_H__
#define __EMA_DATA_H__

#include <map>
#include <vector>
#include <string>

#include <armadillo>

#include "ema/EmaCoil.h"



class EmaData{

public:

  // map that associated an EmaCoil object to an EMA coil id
  std::map<std::string, EmaCoil> emaCoils;
  
  // vector containing all EMA coil ids
  std::vector<std::string> emaCoilIds;
  
  // time stamps of the time frames
  std::vector<double> timeStamps;
  
  // sampling frequency of the EMA data in Hertz
  double samplingFrequency;

};
#endif
