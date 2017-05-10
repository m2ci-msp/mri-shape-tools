#ifndef __FIT_MODEL_ENERGY_H__
#define __FIT_MODEL_ENERGY_H__

#include "optimization/fitmodel/EnergyData.h"
#include "optimization/fitmodel/EnergySettings.h"
#include "optimization/fitmodel/EnergyNeighbor.h"
#include "optimization/fitmodel/EnergyDerivedData.h"
#include "optimization/fitmodel/EnergyDerivedDataUpdate.h"
#include "optimization/fitmodel/EnergyState.h"

namespace fitModel{

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

    EnergyNeighbor* energyNeighbor;
    EnergyDerivedDataUpdate* energyDerivedDataUpdate;
    EnergyState energyState;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
