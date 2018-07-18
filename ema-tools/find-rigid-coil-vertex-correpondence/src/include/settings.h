#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include "optimization/rigidalignment/EnergySettings.h"
#include "optimization/rigidalignment/MinimizerSettings.h"

#include <string>

class Settings {

public:

  // input and output
  std::string emaFile;
  std::string mesh;

  std::vector<std::string> channels;

  std::vector<std::string> subsets;

  std::string outputFile;

  rigidAlignment::MinimizerSettings minimizerSettings;
  rigidAlignment::EnergySettings energySettings;

  double scaleFactor = 1;

  int partitionAmount = 1;
  int partitionIndex = 1;

  double timeStamp = 0;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> emaFileFlag("emaFile", this->emaFile);
    FlagSingle<std::string> meshFlag("mesh", this->mesh);

    FlagSingle<std::string> outputFileFlag("outputFile", this->outputFile);

    FlagList<std::string> subsetsFlag("subsets", this->subsets);

    FlagSingle<double> timeStampFlag("timeStamp", this->timeStamp);

    FlagList<std::string> channelsFlag("channels", this->channels);
    FlagSingle<double> scaleFactorFlag("scaleFactor", this->scaleFactor, true);

    FlagSingle<int> partitionAmountFlag("partitionAmount", this->partitionAmount, true);
    FlagSingle<int> partitionIndexFlag("partitionIndex", this->partitionIndex, true);

    /////////////////////////////////////////////////////////////////////////

    // minimizer settings
    FlagSingle<int> convergenceFactorFlag(
                                          "convergenceFactor",
                                          this->minimizerSettings.convergenceFactor, true);

    FlagSingle<double> projectedGradientToleranceFlag(
                                                      "projectedGradientTolerance",
                                                      this->minimizerSettings.projectedGradientTolerance, true);

    FlagSingle<int> maxFunctionEvalsFlag(
                                         "maxFunctionEvals",
                                         this->minimizerSettings.maxFunctionEvals, true);



    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&emaFileFlag);
    parser.define_flag(&meshFlag);
    parser.define_flag(&outputFileFlag);

    // channels
    parser.define_flag(&channelsFlag);

    // frame index
    parser.define_flag(&timeStampFlag);

    parser.define_flag(&subsetsFlag);

    // scaling
    parser.define_flag(&scaleFactorFlag);

    parser.define_flag(&partitionAmountFlag);
    parser.define_flag(&partitionIndexFlag);

    // minimizer settings
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

    this->minimizerSettings.iterationAmount = 1;
    this->minimizerSettings.noScaling = true;

    this->energySettings.searchStrategy = rigidAlignment::EnergySettings::SearchStrategy::FIXED;

  }

};

#endif
