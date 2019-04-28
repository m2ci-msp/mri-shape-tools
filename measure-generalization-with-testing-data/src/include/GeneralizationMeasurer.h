#ifndef __GENERALIZATION_MEASURER_H__
#define __GENERALIZATION_MEASURER_H__

#include "model/Model.h"
#include "model/ModelBuilder.h"

#include "mesh/MeshMeanDifference.h"

#include "training/SampleDataBase.h"
#include "training/TrainingDataBuilder.h"
#include "training/TensorBuilder.h"

#include "optimization/fitmodel/Energy.h"
#include "optimization/fitmodel/EnergyMinimizer.h"
#include "optimization/fitmodel/MinimizerSettings.h"

#include "ErrorDatabase.h"

class GeneralizationMeasurer{

public:

  /*-----------------------------------------------------------------------*/

  GeneralizationMeasurer(
                         const SampleDataBase& trainingDataBase,
                         const SampleDataBase& testingDataBase) {

    // setup model builder
    TrainingDataBuilder trainingBuilder(trainingDataBase);

    TrainingData data = trainingBuilder.build();

    auto tensor = TensorBuilder::build_tensor(data);

    this->builder.set_faces(trainingDataBase.get_faces());
    this->builder.set_tensor(tensor);
    this->builder.set_origin(data.mean);

    // setup testing data
    this->testingData = testingDataBase.get_all_meshes();

    // setup mode dimensions
    this->phonemeDimension = trainingDataBase.get_phoneme_ids().size();
    this->speakerDimension = trainingDataBase.get_speaker_ids().size();

  }

  /*-----------------------------------------------------------------------*/

  GeneralizationMeasurer& set_energy_settings(
    const fitModel::EnergySettings& energySettings) {

    this->energySettings = energySettings;

    return *this;
  }

  /*-----------------------------------------------------------------------*/

  GeneralizationMeasurer& set_minimizer_settings(
    const fitModel::MinimizerSettings& minimizerSettings) {

    this->minimizerSettings = minimizerSettings;

    return *this;
  }

  /*-----------------------------------------------------------------------*/

  GeneralizationMeasurer& set_min_components( const int minComponents) {

    this->minComponents = minComponents;

    return *this;
  }

  /*-----------------------------------------------------------------------*/

  GeneralizationMeasurer& set_restricted_mode( const int restricted) {

    this->restrictedMode = restricted;

    if( restricted > 0) {

      this->restrictOtherMode = true;

    }

    return *this;
  }

  /*-----------------------------------------------------------------------*/

  ErrorDatabase measure() {

    ErrorDatabase errorDataBase;

    // set total mode dimension for trained models
    const int modeDimension = get_mode_dimension();

    // reduce mode dimensions
    for(int i = modeDimension; i >= this->minComponents; --i) {

      create_truncated_model(i);

      // fit model to all targets
      for(const Mesh& currentTarget: this->testingData) {

        Mesh result = fit_model(currentTarget);

        const double error =
          MeshMeanDifference::compute(result, currentTarget);

        // record average error for current target and component amount
        errorDataBase.add_error(i, error);

      } // end for current target

    } // end i

    return errorDataBase;

  } // end measure

  /*-----------------------------------------------------------------------*/

protected:

  /*-----------------------------------------------------------------------*/

  Mesh fit_model(const Mesh& currentTarget) {

    fitModel::EnergyData energyData(this->currentModel, currentTarget);
    fitModel::Energy energy(energyData, energySettings);
    fitModel::EnergyMinimizer minimizer(energy, minimizerSettings);

    minimizer.minimize();

    return energy.derived_data().source;

  }

  /*-----------------------------------------------------------------------*/

  virtual void create_truncated_model(const int&) = 0;
  virtual int get_mode_dimension() = 0;

  /*-----------------------------------------------------------------------*/

  int speakerDimension;
  int phonemeDimension;

  int minComponents = 1;
  int restrictedMode = 0;
  bool restrictOtherMode = false;

  std::vector<Mesh> testingData;

  /*-----------------------------------------------------------------------*/

  ModelBuilder builder;
  Model currentModel;

  /*-----------------------------------------------------------------------*/

  fitModel::EnergySettings energySettings;
  fitModel::MinimizerSettings minimizerSettings;

  /*-----------------------------------------------------------------------*/

};

#endif
