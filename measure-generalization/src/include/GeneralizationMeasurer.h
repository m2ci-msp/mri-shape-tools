#ifndef __GENERALITY_MEASURER_H__
#define __GENERALITY_MEASURER_H__

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

  GeneralizationMeasurer(const SampleDataBase& database) :
    database(database) {
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

    // setup ids for mode
    set_mode_ids();

    // compute total mode dimension for trained models:
    // total size - 1 -> one id is always excluded
    const int modeDimension = this->modeIds.size() - 1;

    // exclude each id once for building the model
    // and use excluded id as target
    for(const std::string& id: this->modeIds) {

      setup_target_with(id);

      create_model_builder_without_id(id);

      // reduce mode dimensions
      for(int i = modeDimension; i >= this->minComponents; --i) {

        create_truncated_model(i);

        // fit model to all targets
        for(const Mesh& currentTarget: this->currentTargets) {

          Mesh result = fit_model(currentTarget);
          const double error =
            MeshMeanDifference::compute(result, currentTarget);

          // record average error for current target and component amount
          errorDataBase.add_error(i, error);

        } // end for current target

      } // end i

    } // end id

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

  void create_model_builder_without_id(const std::string& id) {

      setup_training_without(id);
      Tensor tensor = TensorBuilder::build_tensor(currentTrainingData);

      this->builder = ModelBuilder();
      this->builder.set_faces(database.get_faces());
      this->builder.set_tensor(tensor);
      this->builder.set_origin(this->currentTrainingData.mean);

  }

  /*-----------------------------------------------------------------------*/

  virtual void create_truncated_model(const int&) = 0;
  virtual void setup_training_without(const std::string&) = 0;
  virtual void setup_target_with(const std::string&) = 0;
  virtual void set_mode_ids() = 0;

  /*-----------------------------------------------------------------------*/

  const SampleDataBase& database;

  int minComponents = 1;
  int restrictedMode = 0;
  bool restrictOtherMode = false;

  std::vector<Mesh> currentTargets;

  std::set<std::string> modeIds;

  /*-----------------------------------------------------------------------*/

  TrainingData currentTrainingData;
  ModelBuilder builder;
  Model currentModel;

  /*-----------------------------------------------------------------------*/

  fitModel::EnergySettings energySettings;
  fitModel::MinimizerSettings minimizerSettings;

  /*-----------------------------------------------------------------------*/

};

#endif
