#ifndef __EXPORT_BOUNDING_BOX_H__
#define __EXPORT_BOUNDING_BOX_H__

#include <fstream>
#include <cfloat>

#include <json/json.h>
#include "ema/EmaCoilAccess.h"
#include "ema/EmaInfo.h"

class ExportBoundingBox{

public:

  ExportBoundingBox(const EmaCoilAccess& coil, const EmaInfo& info) :

    coil(coil),
    info(info) {

  }

  void to(const std::string& fileName) const {

    std::ofstream outFile(fileName);

    arma::vec max({DBL_MAX, DBL_MAX, DBL_MAX});
    arma::vec min = - max;

    for(int i = 0; i < this->info.sample_amount(); ++i) {

      const arma::vec& position = this->coil.position(i);

      for(int j = 0; j < 3; ++j) {

        max(j) = (max(j) > position(j))? max(j) : position(j);
        min(j) = (min(j) < position(j))? min(j) : position(j);

      }

    }

    Json::Value minPosition(Json::objectValue);

    minPosition["x"] = min(0);
    minPosition["y"] = min(1);
    minPosition["z"] = min(2);

    Json::Value maxPosition(Json::objectValue);

    maxPosition["x"] = max(0);
    maxPosition["y"] = max(1);
    maxPosition["z"] = max(2);

    Json::Value root(Json::objectValue);

    root["min"] = minPosition;
    root["max"] = maxPosition;

    outFile << root << std::endl;

    outFile.close();

  }

private:

  const EmaCoilAccess& coil;
  const EmaInfo& info;

};
#endif
