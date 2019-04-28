#ifndef __EMA_INFO_DATA_H__
#define __EMA_INFO_DATA_H__

#include <vector>
#include <string>

class EmaInfoData{

public:

  // time stamps for all time frames
  std::vector<double> timeStamps;
  
  // all coil IDs
  std::vector<std::string> coilIds;
  
  // temporal sampling frequency of the data
  double samplingFrequency;

};
#endif
