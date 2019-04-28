#ifndef __EMA_BUILDER_H__
#define __EMA_BUILDER_H__

#include "ema/EmaData.h"

class EmaBuilder{

public:

  // constructor sets the reference to the main EmaData instance
  EmaBuilder(EmaData& emaData) : emaData(emaData) {

  }

  void from(const std::vector<double>& timeStamps, const double& samplingFrequency, std::vector< std::vector<arma::vec> > positions, const std::vector<std::string> & coilIds) {

    this->emaData.emaInfoData.timeStamps = timeStamps;
    this->emaData.emaInfoData.samplingFrequency = samplingFrequency;
    this->emaData.emaInfoData.coilIds = coilIds;
    this->emaData.emaCoils.clear();

    for(size_t i = 0; i < coilIds.size(); ++i) {

      EmaCoilData data(emaData.emaInfoData);
      data.positions = positions.at(i);
      data.id = coilIds.at(i);
      this->emaData.emaCoils.emplace(coilIds.at(i), EmaCoil(data));

    }

  }

private:

  // reference to main EmaData instance
  EmaData& emaData;

};
#endif
