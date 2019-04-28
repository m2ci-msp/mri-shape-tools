#ifndef __EXPORT_MEAN_POSITION_H__
#define __EXPORT_MEAN_POSITION_H__

#include <fstream>

#include <json/json.h>
#include "ema/EmaCoilAccess.h"
#include "ema/EmaInfo.h"

class ExportMeanPosition{

public:

  ExportMeanPosition(const EmaCoilAccess& coil, const EmaInfo& info) : 
  
    coil(coil),
    info(info) {
  
  }

  void to(const std::string& fileName) const {
  
    std::ofstream outFile(fileName);

    arma::vec mean = arma::zeros(3);
  
    for(int i = 0; i < this->info.sample_amount(); ++i) {
  
      mean += this->coil.position(i);

    }

    mean /= this->info.sample_amount();

    Json::Value jsonPosition(Json::objectValue);

    jsonPosition["x"] = mean(0);
    jsonPosition["y"] = mean(1);
    jsonPosition["z"] = mean(2);

  
    outFile << jsonPosition << std::endl;
  
    outFile.close();
  
  }

private:

  const EmaCoilAccess& coil;
  const EmaInfo& info;

};
#endif
