#ifndef __EXPORT_SPEED_H__
#define __EXPORT_SPEED_H__

#include <fstream>

#include <json/json.h>
#include "ema/EmaCoilData.h"
#include "ema/EmaCoilCalculus.h"
#include "ema/EmaInfo.h"

class ExportSpeed{

public:

  ExportSpeed(const EmaCoilCalculus& coil, const EmaInfo& info) : 
  
    coil(coil),
    info(info) {
  
  }

  void to(const std::string& fileName) const {
  
    std::ofstream outFile(fileName);
  
    Json::Value jsonList(Json::arrayValue);
  
    for(int i = 0; i < this->info.sample_amount(); ++i) {
  
      Json::Value entry(Json::objectValue);
      entry["time"] = this->info.time_stamps().at(i);
      entry["speed"] = arma::norm( this->coil.position_t(i) );
  
      jsonList.append(entry);
  
    }
  
    outFile << jsonList << std::endl;
  
    outFile.close();
  
  }

private:

  const EmaCoilCalculus& coil;
  const EmaInfo& info;

};
#endif
