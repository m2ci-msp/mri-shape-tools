#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"
#include "flags/FlagFixedList.h"
#include "flags/FlagNone.h"

#include "energy/EnergySettings.h"
#include "energy/MinimizerSettings.h"

#include <string>

class Settings {

public:

  // input and output
  std::string target;
  std::string source;
  std::string output;

  arma::vec center;
  int radius;

  lucasKanade::MinimizerSettings minimizerSettings;
  lucasKanade::EnergySettings energySettings;

  Settings(int argc, char* argv[]) {

    std::vector<double> centerList;

    // input and output
    FlagSingle<std::string> targetFlag("target", this->target);
    FlagSingle<std::string> sourceFlag("source", this->source);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagFixedList<double> centerFlag("center", centerList, 3);
    FlagSingle<int> radiusFlag("radius", this->radius);

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

    parser.define_flag(&centerFlag);
    parser.define_flag(&radiusFlag);

    // minimizer settings
    parser.define_flag(&iterationAmountFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

    this->center = arma::vec({centerList.at(0), centerList.at(1), centerList.at(2)});

  }

};

#endif
