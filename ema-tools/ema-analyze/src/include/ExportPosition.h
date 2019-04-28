#ifndef __EXPORT_POSITION_H__
#define __EXPORT_POSITION_H__

#include <fstream>

#include <json/json.h>
#include "ema/EmaCoilData.h"
#include "ema/EmaCoilAccess.h"
#include "ema/EmaInfo.h"

class ExportPosition{

public:

  ExportPosition(const EmaCoilAccess& coil, const EmaInfo& info) : 
  
    coil(coil),
    info(info) {
  
  }

  void to(const std::string& fileName) const {
  
    std::ofstream outFile(fileName);
  
    Json::Value jsonList(Json::arrayValue);
  
    for(int i = 0; i < this->info.sample_amount(); ++i) {
  
      Json::Value entry(Json::objectValue);
      entry["time"] = this->info.time_stamps().at(i);

      entry["x"] = this->coil.position(i)(0);
      entry["y"] = this->coil.position(i)(1);
      entry["z"] = this->coil.position(i)(2);
  
      jsonList.append(entry);
  
    }
  
    outFile << jsonList << std::endl;
  
    outFile.close();
  
  }

private:

  const EmaCoilAccess& coil;
  const EmaInfo& info;

};
#endif
