#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"
#include "flags/FlagNone.h"

#include "optimization/rigidalignment/EnergySettings.h"
#include "optimization/rigidalignment/MinimizerSettings.h"

#include <string>

class Settings {

public:

  // input and output
  std::string target;
  std::string source;
  std::string output;
  std::string landmarks;

  rigidAlignment::MinimizerSettings minimizerSettings;
  rigidAlignment::EnergySettings energySettings;

  bool landmarksPresent = false;

  bool useNoProjection = false;

  bool initializeWithLandmarks = false;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> targetFlag("target", this->target);
    FlagSingle<std::string> sourceFlag("source", this->source);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> landmarksFlag("landmarks", this->landmarks, true);

    /////////////////////////////////////////////////////////////////////////

    // minimizer settings
    FlagSingle<int> iterationAmountFlag(
      "iter", this->minimizerSettings.iterationAmount, true);

    FlagSingle<int> convergenceFactorFlag(
      "convergenceFactor",
      this->minimizerSettings.convergenceFactor, true);

    FlagSingle<double> projectedGradientToleranceFlag(
      "projectedGradientTolerance",
      this->minimizerSettings.projectedGradientTolerance, true);

    FlagSingle<int> maxFunctionEvalsFlag(
      "maxFunctionEvals",
      this->minimizerSettings.maxFunctionEvals, true);

    FlagNone noRotationFlag("noRotation", this->minimizerSettings.noRotation);
    FlagNone noTranslationFlag("noTranslation", this->minimizerSettings.noTranslation);
    FlagNone noScalingFlag("noScaling", this->minimizerSettings.noScaling);

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

    FlagNone useNoProjectionFlag("useNoProjection", this->useNoProjection);

    FlagNone initializeWithLandmarksFlag("initializeWithLandmarks", this->initializeWithLandmarks);

    /////////////////////////////////////////////////////////////////////////

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&targetFlag);
    parser.define_flag(&sourceFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&landmarksFlag);

    // minimizer settings
    parser.define_flag(&iterationAmountFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    // nearest neighbor settings
    parser.define_flag(&maxDistanceFlag);
    parser.define_flag(&maxAngleFlag);
    parser.define_flag(&searchRadiusFlag);
    parser.define_flag(&useNoProjectionFlag);
    parser.define_flag(&initializeWithLandmarksFlag);

    parser.define_flag(&noRotationFlag);
    parser.define_flag(&noTranslationFlag);
    parser.define_flag(&noScalingFlag);

    parser.parse_from_command_line(argc, argv);

    this->energySettings.useProjection = !this->useNoProjection;

    if( searchRadiusFlag.is_present() == true ) {

      this->energySettings.searchStrategy =
        rigidAlignment::EnergySettings::SearchStrategy::ADAPTIVE;

    }

    this->landmarksPresent = landmarksFlag.is_present();

  }

};

#endif
