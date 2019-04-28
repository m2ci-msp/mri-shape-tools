#ifndef __TRACKER_FITTING_H__
#define __TRACKER_FITTING_H__

#include "TrackerData.h"
#include "TrackerState.h"
#include "TrackerLogger.h"

#include "optimization/fitmodel/EnergyMinimizer.h"

class TrackerFitting{

public:

  /*--------------------------------------------------------------------------*/

  TrackerFitting(
    TrackerData& trackerData,
    TrackerState& trackerState,
    TrackerLogger& trackerLogger
    ) :
    trackerData(trackerData),
    trackerState(trackerState),
    trackerLogger(trackerLogger),
    energyData(trackerData.currentModel) {

    this->energy = new fitModel::Energy(this->energyData,
                                        this->trackerData.settings.energySettings);
    this->energyMinimizer =
      new fitModel::EnergyMinimizer(*this->energy,
                                    this->trackerData.settings.minimizerSettings);

  }

  /*--------------------------------------------------------------------------*/

  ~TrackerFitting() {

    delete this->energy;
    delete this->energyMinimizer;

  }

  /*--------------------------------------------------------------------------*/

  // Reinitalizes the fitting data structures
  void reinit() {

    delete this->energy;
    delete this->energyMinimizer;

    this->energyData.update();

    this->energy = new fitModel::Energy(this->energyData,
                                        this->trackerData.settings.energySettings);
    this->energyMinimizer =
      new fitModel::EnergyMinimizer(*this->energy,
                                    this->trackerData.settings.minimizerSettings);

  }

  /*--------------------------------------------------------------------------*/

  void fit() {

    this->energy->data().target = this->trackerData.target;

    this->energy->neighbors().update_for_target();

    if( this->trackerState.firstFrame == true ) {
      fit_first_frame();
    }
    else {
      fit_frame();
    }

    this->trackerState.firstFrame = false;

    // update weights
    this->trackerData.speakerWeight = this->energyData.speakerWeights;
    this->trackerData.phonemeWeight = this->energyData.phonemeWeights;

    // update energy value
    this->trackerData.energy = this->energy->state().energy;

    // log current state
    this->trackerLogger.log();

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void fit_first_frame() {

    const double speakerSmoothnessWeight =
      this->trackerData.settings.energySettings.weights.at("speakerSmoothnessTerm");

    const double phonemeSmoothnessWeight =
      this->trackerData.settings.energySettings.weights.at("phonemeSmoothnessTerm");

    // set weights to 0 for first frame
    this->energy->settings().weights.at("speakerSmoothnessTerm") = 0;
    this->energy->settings().weights.at("phonemeSmoothnessTerm") = 0;

    fit_frame();

    // restore old weights
    this->energy->settings().weights.at("speakerSmoothnessTerm") =
      speakerSmoothnessWeight;

    this->energy->settings().weights.at("phonemeSmoothnessTerm") =
      phonemeSmoothnessWeight;

    this->trackerState.firstFrame = false;

  }

  /*--------------------------------------------------------------------------*/

  void fit_frame() {

    this->energyMinimizer->minimize();

  }

  /*--------------------------------------------------------------------------*/

  TrackerData& trackerData;
  TrackerState& trackerState;
  TrackerLogger& trackerLogger;

  /*--------------------------------------------------------------------------*/

  fitModel::Energy* energy;
  fitModel::EnergyMinimizer* energyMinimizer;

  fitModel::EnergyData energyData;

  /*--------------------------------------------------------------------------*/

};

#endif
