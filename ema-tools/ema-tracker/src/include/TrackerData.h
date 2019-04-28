#ifndef __TRACKER_DATA_H__
#define __TRACKER_DATA_H__

#include <vector>
#include <armadillo>

#include "model/Model.h"
#include "optimization/fitmodel/EnergySettings.h"

class TrackerData{

public:
  TrackerData(
    const Model& model,
    const Settings& settings
    ) : originalModel(model), currentModel(model), settings(settings) {
  }

  // current model vertex ids corresponding to the received points
  std::vector<int> sourceIds;

  // original multilinear model
  Model originalModel;

  // current model (might be truncated)
  Model currentModel;

  // timestamps
  double currentTime = 0;

  // current weights
  arma::vec speakerWeight;
  arma::vec phonemeWeight;

  // fixed speaker weights (optional)
  arma::vec fixedSpeakerWeights;

  // settings for fitting
  Settings settings;

  // current energy
  double energy;

  Mesh target;

};
#endif
