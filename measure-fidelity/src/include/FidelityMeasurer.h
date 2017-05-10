#ifndef __FIDELITY_MEASURER_H__
#define __FIDELITY_MEASURER_H__

#include "model/Model.h"

#include "mesh/MeshMeanDifference.h"

#include "training/SampleDataBase.h"

#include "optimization/fitmodel/Energy.h"
#include "optimization/fitmodel/EnergyMinimizer.h"
#include "optimization/fitmodel/MinimizerSettings.h"

class FidelityMeasurer{

public:

  /*-----------------------------------------------------------------------*/

  FidelityMeasurer(const Model& model, const SampleDataBase& database) :
    model(model), database(database) {
  }

  /*-----------------------------------------------------------------------*/

  FidelityMeasurer& set_energy_settings(
    const fitModel::EnergySettings& energySettings) {

    this->energySettings = energySettings;

    return *this;
  }

  /*-----------------------------------------------------------------------*/

  FidelityMeasurer& set_minimizer_settings(
    const fitModel::MinimizerSettings& minimizerSettings) {

    this->minimizerSettings = minimizerSettings;

    return *this;
  }

  /*-----------------------------------------------------------------------*/

  void measure(double& mean, double& standardDeviation) {

    std::vector<Mesh> targets = this->database.get_all_meshes();

    arma::vec errors = arma::zeros(targets.size());

    // fit model to all targets
    for(unsigned int i = 0; i < targets.size(); ++i) {

      const Mesh currentTarget = targets.at(i);
      Mesh result = fit_model(currentTarget);
      errors(i) = MeshMeanDifference::compute(result, currentTarget);

    } // end for current target

    mean = arma::mean(errors);
    standardDeviation = arma::stddev(errors, 1);

  } // end measure

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  Mesh fit_model(const Mesh& currentTarget) {

    fitModel::EnergyData energyData(this->model, currentTarget);
    fitModel::Energy energy(energyData, energySettings);
    fitModel::EnergyMinimizer minimizer(energy, minimizerSettings);

    minimizer.minimize();

    return energy.derived_data().source;

  }

  /*-----------------------------------------------------------------------*/

  Model model;

  /*-----------------------------------------------------------------------*/

  const SampleDataBase& database;

  /*-----------------------------------------------------------------------*/

  fitModel::EnergySettings energySettings;
  fitModel::MinimizerSettings minimizerSettings;

  /*-----------------------------------------------------------------------*/

};

#endif
