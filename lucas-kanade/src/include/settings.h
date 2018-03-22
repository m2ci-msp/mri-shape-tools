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
  std::string output;

  std::string mesh;

  double offset = 1;
  double profileLength = 5;

  lucasKanade::MinimizerSettings minimizerSettings;
  lucasKanade::EnergySettings energySettings;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> targetFlag("target", this->target);
    FlagSingle<std::string> sourceFlag("source", this->source);
    FlagSingle<std::string> outputFlag("output", this->output);

    FlagSingle<std::string> meshFlag("mesh", this->mesh);

    FlagSingle<double> offsetFlag("offset", this->offset, true);
    FlagSingle<double> profileLengthFlag("profileLength", this->profileLength, true);

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

    /////////////////////////////////////////////////////////////////////////

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&targetFlag);
    parser.define_flag(&sourceFlag);
    parser.define_flag(&outputFlag);

    parser.define_flag(&meshFlag);
    parser.define_flag(&offsetFlag);
    parser.define_flag(&profileLengthFlag);

    // minimizer settings
    parser.define_flag(&iterationAmountFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
