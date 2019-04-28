#ifndef __CORRESPONDENCE_SUBSET_FINDER_BUILDER_H__
#define __CORRESPONDENCE_SUBSET_FINDER_BUILDER_H__

#include <vector>
#include <stdexcept>

#include <armadillo>

#include "mesh/Mesh.h"

#include "optimization/rigidalignment/EnergySettings.h"
#include "optimization/rigidalignment/MinimizerSettings.h"

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

  CorrespondenceSubsetFinderBuilder& set_energy_settings(const rigidAlignment::EnergySettings& energySettings) {
  
    this->energySettings = energySettings;
    this->energySettingsSet = true;
    return *this;
  
  }
  
  
  CorrespondenceSubsetFinderBuilder& set_minimizer_settings(const rigidAlignment::MinimizerSettings& minimizerSettings) {
  
    this->minimizerSettings = minimizerSettings;
    this->minimizerSettingsSet = true;
    return *this;
  
  }

  CorrespondenceSubsetFinderBuilder& set_mesh(const Mesh& mesh) {

    this->mesh = mesh;
    this->meshSet = true;
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

    return CorrespondenceSubsetFinder(this->emaData, this->energySettings, this->minimizerSettings, this->mesh, this->subsets, this->partitionIndex, this->partitionAmount);

  }

private:

  bool verify() const {

    return (
            emaDataSet &&
            energySettingsSet &&
            minimizerSettingsSet &&
            meshSet &&
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
  rigidAlignment::EnergySettings energySettings;
  
  // settings for the minimizer
  rigidAlignment::MinimizerSettings minimizerSettings;
 
  Mesh mesh;

  int partitionIndex;
  int partitionAmount;

  // indicators if a specific needed member was set
  bool subsetsSet = false;
  bool emaDataSet = false;
  bool energySettingsSet = false;
  bool minimizerSettingsSet = false;
  bool meshSet = false;
  bool partitionIndexSet = false;
  bool partitionAmountSet = false;

};

#endif
