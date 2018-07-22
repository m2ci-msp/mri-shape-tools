#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include "optimization/fitmodel/EnergySettings.h"
#include "optimization/fitmodel/MinimizerSettings.h"

#include <string>

class Settings {

public:

  // input and output
  std::string emaFile;
  std::string model;

  std::vector<std::string> channels;

  std::vector<std::string> subsets;

  std::string emaModifications;
  bool applyModifications = false;

  std::string outputFile;

  fitModel::MinimizerSettings minimizerSettings;
  fitModel::EnergySettings energySettings;

  int partitionAmount = 1;
  int partitionIndex = 1;

  int timeFrame = 0;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> emaFileFlag("emaFile", this->emaFile);
    FlagSingle<std::string> modelFlag("model", this->model);

    FlagSingle<std::string> outputFileFlag("outputFile", this->outputFile);

    FlagList<std::string> subsetsFlag("subsets", this->subsets);

    // ema modifications
    FlagSingle<std::string> emaModificationsFlag("emaModifications", this->emaModifications, true);

    FlagSingle<int> timeFrameFlag("timeFrame", this->timeFrame);

    FlagList<std::string> channelsFlag("channels", this->channels);

    FlagSingle<int> partitionAmountFlag("partitionAmount", this->partitionAmount, true);
    FlagSingle<int> partitionIndexFlag("partitionIndex", this->partitionIndex, true);

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
    parser.define_flag(&emaFileFlag);
    parser.define_flag(&modelFlag);
    parser.define_flag(&outputFileFlag);

    // channels
    parser.define_flag(&channelsFlag);

    // frame index
    parser.define_flag(&timeFrameFlag);

    // subsets
    parser.define_flag(&subsetsFlag);

    // ema modifications
    parser.define_flag(&emaModificationsFlag);

    parser.define_flag(&partitionAmountFlag);
    parser.define_flag(&partitionIndexFlag);

    // minimizer settings
    parser.define_flag(&priorSizeFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

    // use only one iteration
    this->minimizerSettings.iterationAmount = 1;
    // and fixed correspondences
    this->energySettings.searchStrategy =
      fitModel::EnergySettings::SearchStrategy::FIXED;

    this->applyModifications = emaModificationsFlag.is_present();

  }

};

#endif
