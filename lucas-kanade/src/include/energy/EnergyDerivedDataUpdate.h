#ifndef __LUCAS_KANADE_ENERGY_DERIVED_DATA_UPDATE_H__
#define __LUCAS_KANADE_ENERGY_DERIVED_DATA_UPDATE_H__

#include "energy/EnergyData.h"
#include "energy/TransformationEnum.h"
#include "energy/ITKWrapper.h"

namespace lucasKanade{

  class EnergyDerivedDataUpdate {

  private:

    EnergyData& energyData;
    EnergySettings& energySettings;

  public:

    /*--------------------------------------------------------------------------*/

    EnergyDerivedDataUpdate(
                            EnergyData& energyData,
                            EnergySettings& energySettings
                            ) :
      energyData(energyData),
      energySettings(energySettings) {
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

      ITKWrapper::vnl_vector_to_transformation(this->energyData.transformation,
                                               this->energyData.center,
                                               this->energyData.transformationMatrix);

    }

    /*--------------------------------------------------------------------------*/

    void update_deformed_template() {

      DeformedTemplate& deformedTemplate = this->energyData.deformedTemplate;

      deformedTemplate.compute(this->energyData.transformationMatrix);

    }

    /*--------------------------------------------------------------------------*/

  };
}

#endif
