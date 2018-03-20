#ifndef __LUCAS_KANADE_ENERGY_DERIVED_DATA_UPDATE_H__
#define __LUCAS_KANADE_ENERGY_DERIVED_DATA_UPDATE_H__

#include "energy/EnergyData.h"
#include "energy/EnergyDerivedData.h"
#include "energy/TransformationEnum.h"
#include "energy/ITKWrapper.h"

namespace lucasKanade{

  class EnergyDerivedDataUpdate {

  private:

    EnergyData& energyData;
    EnergyDerivedData& energyDerivedData;
    EnergySettings& energySettings;

    std::vector<arma::vec> transformedLocations;

  public:

    /*--------------------------------------------------------------------------*/

    EnergyDerivedDataUpdate(
                            EnergyData& energyData,
                            EnergyDerivedData& energyDerivedData,
                            EnergySettings& energySettings
                            ) :
      energyData(energyData),
      energyDerivedData(energyDerivedData),
      energySettings(energySettings) {
    }

    /*--------------------------------------------------------------------------*/

    void for_template() {

      this->energyDerivedData.templateImage.clear();

      for(int i = this->energyData.minX; i < this->energyData.maxX; ++i) {
        for(int j = this->energyData.minY; j < this->energyData.maxY; ++j) {
          for(int k = this->energyData.minZ; k < this->energyData.maxZ; ++k) {

            this->energyDerivedData.templateImage.push_back( this->energyData.templateImage.access().at_grid(i, j, k) );

          }
        }
      }

    }

    /*--------------------------------------------------------------------------*/

    // updates all data structures depending on the transformation parameters
    void for_transformation() {

      compute_transformation_matrix();

      update_deformed_template();

    }

    /*--------------------------------------------------------------------------*/

  private:

    void compute_transformation_matrix() {

      ITKWrapper::vnl_vector_to_transformation(this->energyDerivedData.transformation,
                                               this->energyData.center,
                                               this->energyDerivedData.transformationMatrix);

    }

    /*--------------------------------------------------------------------------*/

    void update_deformed_template() {

      DeformedTemplate& deformedTemplate = this->energyDerivedData.deformedTemplate;

      deformedTemplate.compute(this->energyDerivedData.transformationMatrix);

    }

    /*--------------------------------------------------------------------------*/

  };
}
#endif
