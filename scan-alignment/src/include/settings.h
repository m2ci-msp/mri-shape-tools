#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"
#include "flags/FlagNone.h"

#include "EnergySettings.h"
#include "MinimizerSettings.h"

#include <string>

class Settings {

public:

  // input and output
  std::string sourceMesh;
  std::string sourceScan;
  std::string targetScan;
  std::string output;
  std::string minimizerOutputFile;

  bool outputMinimizer = false;

  MinimizerSettings minimizerSettings;
  EnergySettings energySettings;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> sourceMeshFlag("sourceMesh", this->sourceMesh);
    FlagSingle<std::string> sourceScanFlag("sourceScan", this->sourceScan);
    FlagSingle<std::string> targetScanFlag("targetScan", this->targetScan);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> minimizerOutputFlag("minimizerOutput", this->minimizerOutputFile, true);

    /////////////////////////////////////////////////////////////////////////

    // minimizer settings
    FlagSingle<int> iterationAmountFlag(
      "iter", this->minimizerSettings.iterationAmount, true);

    FlagSingle<double> translationLimitFlag(
      "translationLimit", this->minimizerSettings.translationLimit, true);

    FlagSingle<double> rotationLimitFlag(
      "rotationLimit", this->minimizerSettings.rotationLimit, true);


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

    // color profile settings
    FlagSingle<double> profileLengthFlag(
      "profileLength", this->energySettings.profileLength, true);

    FlagSingle<double> profileOffsetFlag(
      "profileOffset", this->energySettings.profileOffset, true);

    /////////////////////////////////////////////////////////////////////////

    // derivative approximation settings
    FlagSingle<double> translationStepFlag(
      "translationStep", this->energySettings.translationStep, true);

    FlagSingle<double> rotationStepFlag(
      "rotationStep", this->energySettings.rotationStep, true);

    /////////////////////////////////////////////////////////////////////////

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&sourceMeshFlag);
    parser.define_flag(&sourceScanFlag);
    parser.define_flag(&targetScanFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&minimizerOutputFlag);

    // minimizer settings
    parser.define_flag(&iterationAmountFlag);
    parser.define_flag(&translationLimitFlag);
    parser.define_flag(&rotationLimitFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    // color profile settings
    parser.define_flag(&profileLengthFlag);
    parser.define_flag(&profileOffsetFlag);

    // derivative approximation settings
    parser.define_flag(&translationStepFlag);
    parser.define_flag(&rotationStepFlag);

    parser.parse_from_command_line(argc, argv);

    this->outputMinimizer = minimizerOutputFlag.is_present();

  }

};

#endif
