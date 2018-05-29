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

  std::string outputFile;

  fitModel::MinimizerSettings minimizerSettings;
  fitModel::EnergySettings energySettings;

  double shiftX = 0;
  double shiftY = 0;
  double shiftZ = 0;

  bool enforceMidsagittal = false;

  double scaleFactor = 1;

  int partitionAmount = 1;
  int partitionIndex = 1;

  int timeFrame = 0;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> emaFileFlag("emaFile", this->emaFile);
    FlagSingle<std::string> modelFlag("model", this->model);

    FlagSingle<std::string> outputFileFlag("outputFile", this->outputFile);

    FlagList<std::string> subsetsFlag("subsets", this->subsets);

    FlagSingle<int> timeFrameFlag("timeFrame", this->timeFrame);

    FlagList<std::string> channelsFlag("channels", this->channels);

    // for shifting
    FlagSingle<double> shiftXFlag("shiftX", this->shiftX, true);
    FlagSingle<double> shiftYFlag("shiftY", this->shiftY, true);
    FlagSingle<double> shiftZFlag("shiftZ", this->shiftZ, true);

    // for scaling
    FlagSingle<double> scaleFactorFlag("scaleFactor", this->scaleFactor, true);

    // for enforcing midsagittal behaviour of all coils
    FlagNone enforceMidsagittalFlag("enforceMidsagittal", this->enforceMidsagittal);

    // for accessing the partition
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

    parser.define_flag(&subsetsFlag);

    // scaling
    parser.define_flag(&scaleFactorFlag);

    // partition access
    parser.define_flag(&partitionAmountFlag);
    parser.define_flag(&partitionIndexFlag);

    // shifting
    parser.define_flag(&shiftXFlag);
    parser.define_flag(&shiftYFlag);
    parser.define_flag(&shiftZFlag);

    // minimizer settings
    parser.define_flag(&priorSizeFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    // enforce midsagittal
    parser.define_flag(&enforceMidsagittalFlag);

    parser.parse_from_command_line(argc, argv);

    // use only one iteration
    this->minimizerSettings.iterationAmount = 1;
    // and fixed correspondences
    this->energySettings.searchStrategy =
      fitModel::EnergySettings::SearchStrategy::FIXED;

  }

};

#endif
