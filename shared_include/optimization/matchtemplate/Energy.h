#ifndef __MATCH_TEMPLATE_ENERGY_H__
#define __MATCH_TEMPLATE_ENERGY_H__

#include "optimization/matchtemplate/EnergyData.h"
#include "optimization/matchtemplate/EnergySettings.h"
#include "optimization/matchtemplate/EnergyState.h"
#include "optimization/matchtemplate/EnergyNeighbor.h"
#include "optimization/matchtemplate/EnergyDerivedData.h"
#include "optimization/matchtemplate/EnergyDerivedDataUpdate.h"

namespace matchTemplate{

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

      this->energyDerivedData.source = this->energyData.source;

      this->energyNeighbor =
        new EnergyNeighbor(
          this->energyData, this->energyDerivedData, this->energySettings);

      this->energyDerivedDataUpdate =
        new EnergyDerivedDataUpdate(
          this->energyData, this->energyDerivedData, this->energySettings);

    }

    /*--------------------------------------------------------------------------*/

    ~Energy() {
      delete this->energyNeighbor;
      delete this->energyDerivedDataUpdate;
    }

    /*--------------------------------------------------------------------------*/

    EnergyData& data() {
      return this->energyData;
    }

    /*--------------------------------------------------------------------------*/

    EnergyDerivedData& derived_data() {
      return this->energyDerivedData;
    }

    /*--------------------------------------------------------------------------*/

    EnergyDerivedDataUpdate& update() {
      return *this->energyDerivedDataUpdate;
    }

    /*--------------------------------------------------------------------------*/

    EnergyNeighbor& neighbors() {
      return *this->energyNeighbor;
    }

    /*--------------------------------------------------------------------------*/

    EnergySettings& settings() {
      return this->energySettings;
    }

    /*--------------------------------------------------------------------------*/

    EnergyState& state() {
      return this->energyState;
    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    EnergyData& energyData;
    EnergyDerivedData energyDerivedData;
    EnergySettings energySettings;
    EnergyState energyState;

    EnergyNeighbor* energyNeighbor;
    EnergyDerivedDataUpdate* energyDerivedDataUpdate;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
