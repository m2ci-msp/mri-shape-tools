#ifndef __RIGID_ALIGNMENT_ENERGY_H__
#define __RIGID_ALIGNMENT_ENERGY_H__

#include "optimization/rigidalignment/EnergyData.h"
#include "optimization/rigidalignment/EnergySettings.h"
#include "optimization/rigidalignment/EnergyNeighbor.h"
#include "optimization/rigidalignment/EnergyDerivedData.h"
#include "optimization/rigidalignment/EnergyDerivedDataUpdate.h"
#include "optimization/rigidalignment/EnergyState.h"

namespace rigidAlignment{

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

    EnergyNeighbor* energyNeighbor;
    EnergyDerivedDataUpdate* energyDerivedDataUpdate;
    EnergyState energyState;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
