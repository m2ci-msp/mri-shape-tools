#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"
#include "flags/FlagNone.h"

#include "optimization/fitmodel/EnergySettings.h"
#include "optimization/fitmodel/MinimizerSettings.h"

#include <string>

class Settings {

public:

  // input and output
  std::string target;
  std::string model;
  std::string output;
  std::string landmarks;

  fitModel::MinimizerSettings minimizerSettings;
  fitModel::EnergySettings energySettings;

  double landmarkTermWeight = 1;

  bool fixedNeighbors = false;
  bool useNoProjection = false;
  bool landmarksPresent = false;
  bool useLandmarksOnlyForInitialization = false;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> targetFlag("target", this->target);
    FlagSingle<std::string> modelFlag("model", this->model);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> landmarksFlag("landmarks", this->landmarks, true);

    /////////////////////////////////////////////////////////////////////////

    // energy settings
    FlagSingle<double> landmarkTermWeightFlag("landmarkTermWeight", this->landmarkTermWeight, true);

    /////////////////////////////////////////////////////////////////////////

    // minimizer settings
    FlagSingle<int> iterationAmountFlag(
      "iter", this->minimizerSettings.iterationAmount, true);
    FlagSingle<double> priorSizeFlag(
      "priorSize", this->minimizerSettings.priorSize, true);

    FlagSingle<int> convergenceFactorFlag(
      "convergenceFactor",
      this->minimizerSettings.convergenceFactor, true);

    FlagSingle<double> projectedGradientToleranceFlag(
      "projectedGradientTolerance",
      this->minimizerSettings.projectedGradientTolerance, true);

    FlagSingle<int> maxFunctionEvalsFlag(
      "maxFunctionEvals",
      this->minimizerSettings.maxFunctionEvals, true);

    /////////////////////////////////////////////////////////////////////////

    // nearest neighbor settings
    FlagSingle<double> maxDistanceFlag(
      "maxDistance", this->energySettings.maxDistance, true);

    FlagSingle<double> maxAngleFlag(
      "maxAngle", this->energySettings.maxAngle, true);

    // set default value
    this->energySettings.searchRadius = 0;
    FlagSingle<double> searchRadiusFlag(
      "searchRadius", this->energySettings.searchRadius, true);

    FlagNone fixedNeighborsFlag("fixedNeighbors", this->fixedNeighbors);
    FlagNone useNoProjectionFlag("useNoProjection", this->useNoProjection);
    FlagNone useLandmarksOnlyForInitializationFlag(
      "useLandmarksOnlyForInitialization",
      this->useLandmarksOnlyForInitialization);

    /////////////////////////////////////////////////////////////////////////

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&targetFlag);
    parser.define_flag(&modelFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&landmarksFlag);

    // energy settings
    parser.define_flag(&landmarkTermWeightFlag);

    // minimizer settings
    parser.define_flag(&useLandmarksOnlyForInitializationFlag);
    parser.define_flag(&iterationAmountFlag);
    parser.define_flag(&priorSizeFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    // nearest neighbor settings
    parser.define_flag(&maxDistanceFlag);
    parser.define_flag(&maxAngleFlag);
    parser.define_flag(&searchRadiusFlag);
    parser.define_flag(&fixedNeighborsFlag);
    parser.define_flag(&useNoProjectionFlag);

    parser.parse_from_command_line(argc, argv);

    this->energySettings.useProjection = !this->useNoProjection;

    if( this->fixedNeighbors == true) {
      this->energySettings.searchStrategy =
        fitModel::EnergySettings::SearchStrategy::FIXED;
    }
    else if( searchRadiusFlag.is_present() == true ) {
      this->energySettings.searchStrategy =
        fitModel::EnergySettings::SearchStrategy::ADAPTIVE;
    }

    this->landmarksPresent = landmarksFlag.is_present();

  }

};

#endif
