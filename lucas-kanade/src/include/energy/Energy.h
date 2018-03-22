#ifndef __LUCAS_KANADE_ENERGY_H__
#define __LUCAS_KANADE_ENERGY_H__

#include "energy/EnergyData.h"
#include "energy/EnergySettings.h"
#include "energy/EnergyDerivedDataUpdate.h"
#include "energy/EnergyState.h"

namespace lucasKanade{

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

      this->energyDerivedDataUpdate =
        new EnergyDerivedDataUpdate(
          this->energyData, this->energySettings);

      this->energyData.transformation.resize(energyData.transformationAmount, 0);

    }

    /*--------------------------------------------------------------------------*/

    ~Energy() {
      delete this->energyDerivedDataUpdate;
    }

    /*--------------------------------------------------------------------------*/

    EnergyData& data() {
      return this->energyData;
    }

    /*--------------------------------------------------------------------------*/

    EnergyDerivedDataUpdate& update() {
      return *this->energyDerivedDataUpdate;
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
    EnergySettings energySettings;

    EnergyDerivedDataUpdate* energyDerivedDataUpdate;
    EnergyState energyState;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
