#ifndef __EMA_RENAME_H__
#define __EMA_RENAME_H__

#include <algorithm>
#include <string>

#include "ema/EmaData.h"
#include "ema/EmaCoil.h"

class EmaRename{

private:

  EmaData& emaData;

public:

  EmaRename(EmaData& emaData) : emaData(emaData) {

  }

  void with(const std::map<std::string, std::string>& mapping) {

    const std::vector<std::string>& originalIds = this->emaData.emaInfoData.coilIds;
    const std::map<std::string, EmaCoil>& originalMap = this->emaData.emaCoils;

    std::vector<std::string> newIds;
    std::map<std::string, EmaCoil> newMap;

    for(const std::string& id: originalIds) {

      if( mapping.find(id) != mapping.end() )   {

        const std::string& newId = mapping.at(id);

        newMap.emplace(newId, originalMap.at(id));
        newIds.push_back(newId);

      }
      else {

        newMap.emplace(id, originalMap.at(id));
        newIds.push_back(id);

      }

    }

    this->emaData.emaInfoData.coilIds = newIds;
    this->emaData.emaCoils = newMap;

  }

};
#endif
