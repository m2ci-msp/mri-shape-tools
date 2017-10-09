#ifndef __CORRESPONDENCE_SUBSET_FINDER_BUILDER_H__
#define __CORRESPONDENCE_SUBSET_FINDER_BUILDER_H__

#include <vector>
#include <stdexcept>

#include <armadillo>

#include "optimization/fitmodel/EnergySettings.h"
#include "optimization/fitmodel/MinimizerSettings.h"

#include "CorrespondenceSubsetFinder.h"

class CorrespondenceSubsetFinderBuilder{

public:

  CorrespondenceSubsetFinderBuilder& set_subsets(const std::vector< std::vector<unsigned int> >& subsets) {
  
    this->subsets = subsets;
    this->subsetsSet = true;
    return *this;
  
  }
  
  CorrespondenceSubsetFinderBuilder& set_ema_data(const std::vector<arma::vec>& emaData) {
  
    this->emaData = emaData;
    this->emaDataSet = true;
    return *this;
  
  }
  
  CorrespondenceSubsetFinderBuilder& set_energy_settings(const fitModel::EnergySettings& energySettings) {
  
    this->energySettings = energySettings;
    this->energySettingsSet = true;
    return *this;
  
  }
  
  
  CorrespondenceSubsetFinderBuilder& set_minimizer_settings(const fitModel::MinimizerSettings& minimizerSettings) {
  
    this->minimizerSettings = minimizerSettings;
    this->minimizerSettingsSet = true;
    return *this;
  
  }
  
  CorrespondenceSubsetFinderBuilder& set_model(const Model& model) {
  
    this->model = model;
    this->modelSet = true;
    return *this;
  
  }
  
  CorrespondenceSubsetFinderBuilder& set_partition_index(const int& partitionIndex) {
  
    this->partitionIndex = partitionIndex;
    this->partitionIndexSet = true;
    return *this;
  
  }
  
  CorrespondenceSubsetFinderBuilder& set_partition_amount(const int& partitionAmount) {
  
    this->partitionAmount = partitionAmount;
    this->partitionAmountSet = true;
    return *this;
  
  }

  CorrespondenceSubsetFinder build() const {
  
    if(verify() == false) {
  
      throw std::runtime_error("Can not construct Correspondence Subset Finder - not all needed information is present.");
    }
  
    return CorrespondenceSubsetFinder(this->emaData, this->energySettings, this->minimizerSettings, this->model, this->subsets, this->partitionIndex, this->partitionAmount);
  
  }

private:

  bool verify() const {
  
    return (
            emaDataSet &&
            energySettingsSet &&
            minimizerSettingsSet &&
            modelSet &&
            subsetsSet &&
            partitionIndexSet &&
            partitionAmountSet
            );
  
  }

  // vertex indices of all subsets
  std::vector< std::vector<unsigned int> > subsets;
  
  // target coil positions
  std::vector<arma::vec> emaData;
  
  // energy settings
  fitModel::EnergySettings energySettings;
  
  // settings for the minimizer
  fitModel::MinimizerSettings minimizerSettings;
  
  // base multilinear model
  Model model;
  
  int partitionIndex;
  int partitionAmount;
  
  // indicators if a specific needed member was set
  bool subsetsSet = false;
  bool emaDataSet = false;
  bool energySettingsSet = false;
  bool minimizerSettingsSet = false;
  bool modelSet = false;
  bool partitionIndexSet = false;
  bool partitionAmountSet = false;

};

#endif
