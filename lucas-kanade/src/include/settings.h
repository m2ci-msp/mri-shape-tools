#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"
#include "flags/FlagNone.h"

#include "energy/EnergySettings.h"
#include "energy/MinimizerSettings.h"

class Settings {

public:

  // input and output
  std::string target;
  std::string source;
  std::string landmarkFile;
  std::string output;


  lucasKanade::MinimizerSettings minimizerSettings;
  lucasKanade::EnergySettings energySettings;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> targetFlag("target", this->target);
    FlagSingle<std::string> sourceFlag("source", this->source);
    FlagSingle<std::string> landmarkFileFlag("landmarkFile", this->landmarkFile);
    FlagSingle<std::string> outputFlag("output", this->output);

    /////////////////////////////////////////////////////////////////////////

    // minimizer settings
    FlagSingle<int> iterationAmountFlag(
      "iterationAmount", this->minimizerSettings.iterationAmount, true);

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

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&sourceFlag);
    parser.define_flag(&targetFlag);
    parser.define_flag(&landmarkFileFlag);
    parser.define_flag(&outputFlag);

    // minimizer settings
    parser.define_flag(&iterationAmountFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
