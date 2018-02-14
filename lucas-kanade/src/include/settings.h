#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>
#include <armadillo>

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

  arma::vec center;
  arma::vec radius;

  lucasKanade::MinimizerSettings minimizerSettings;
  lucasKanade::EnergySettings energySettings;

  Settings(int argc, char* argv[]) {

    double centerX, centerY, centerZ;
    double radiusX, radiusY, radiusZ;

    // input and output
    FlagSingle<std::string> targetFlag("target", this->target);
    FlagSingle<std::string> sourceFlag("source", this->source);
    FlagSingle<std::string> outputFlag("output", this->output);

    FlagSingle<double> centerXFlag("centerX", centerX);
    FlagSingle<double> centerYFlag("centerY", centerY);
    FlagSingle<double> centerZFlag("centerZ", centerZ);

    FlagSingle<double> radiusXFlag("radiusX", radiusX);
    FlagSingle<double> radiusYFlag("radiusY", radiusY);
    FlagSingle<double> radiusZFlag("radiusZ", radiusZ);

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

    parser.define_flag(&centerXFlag);
    parser.define_flag(&centerYFlag);
    parser.define_flag(&centerZFlag);
    parser.define_flag(&radiusXFlag);
    parser.define_flag(&radiusYFlag);
    parser.define_flag(&radiusZFlag);

    // minimizer settings
    parser.define_flag(&iterationAmountFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

    this->center = arma::vec({centerX, centerY, centerZ});
    this->radius = arma::vec({radiusX, radiusY, radiusZ});

  }

};

#endif
