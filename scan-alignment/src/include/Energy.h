#ifndef __ENERGY_H__
#define __ENERGY_H__

#include "EnergyData.h"
#include "EnergySettings.h"
#include "EnergyInfo.h"

/* composition class containing data structures relevant for the energy */
class Energy{

public:

  /*--------------------------------------------------------------------------*/

  Energy(
    EnergyData& energyData,
    EnergySettings& energySettings
    ) :
    energyData(energyData),
    energySettings(energySettings) {

  }

  /*--------------------------------------------------------------------------*/

  ~Energy() {
  }

  /*--------------------------------------------------------------------------*/

  EnergyData& data() {
    return this->energyData;
  }

  /*--------------------------------------------------------------------------*/

  EnergySettings& settings() {
    return this->energySettings;
  }

  /*--------------------------------------------------------------------------*/

  EnergyInfo& info() {
    return this->energyInfo;
  }
  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  EnergyData& energyData;
  EnergySettings energySettings;
  EnergyInfo energyInfo;

  /*--------------------------------------------------------------------------*/

};

#endif
