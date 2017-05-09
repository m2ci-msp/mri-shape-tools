#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

#include "optimization/fitmodel/MinimizerSettings.h"
#include "optimization/fitmodel/EnergySettings.h"

#include <string>

class Settings {

public:

  // input and output
  std::string model;
  std::string meshes;
  std::string output;

  fitModel::MinimizerSettings minimizerSettings;
  fitModel::EnergySettings energySettings;

  Settings(int argc, char* argv[]) {

    // fix correspondences
    this->energySettings.searchStrategy =
      fitModel::EnergySettings::SearchStrategy::FIXED;

    this->energySettings.useProjection = false;

    // use one iteration
    this->minimizerSettings.iterationAmount = 1;

    /////////////////////////////////////////////////////////////////////////

    FlagSingle<std::string> modelFlag("model", this->model);
    FlagSingle<std::string> meshesFlag("meshes", this->meshes);
    FlagSingle<std::string> outputFlag("output", this->output);

    /////////////////////////////////////////////////////////////////////////

    // minimizer settings

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

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&modelFlag);
    parser.define_flag(&meshesFlag);
    parser.define_flag(&outputFlag);

    // minimizer settings
    parser.define_flag(&priorSizeFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
