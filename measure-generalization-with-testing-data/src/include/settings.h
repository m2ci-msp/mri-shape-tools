#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string trainingData;
  std::string testingData;

  std::string output;

  int restrictedSpeaker = 0;
  int restrictedPhoneme = 0;
  int minComponents = 1;

  fitModel::MinimizerSettings minimizerSettings;
  fitModel::EnergySettings energySettings;

  Settings(int argc, char* argv[]) {

    // fix correspondences
    this->energySettings.searchStrategy =
      fitModel::EnergySettings::SearchStrategy::FIXED;

    this->energySettings.useProjection = false;

    // use one iteration
    this->minimizerSettings.iterationAmount = 1;

    // deactivate PCA behaviour
    this->minimizerSettings.noSpeakerPCA = true;
    this->minimizerSettings.noPhonemePCA = true;


    /////////////////////////////////////////////////////////////////////////

    FlagSingle<std::string> trainingDataFlag("trainingData", this->trainingData);
    FlagSingle<std::string> testingDataFlag("testingData", this->testingData);

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

    FlagSingle<int> minComponentsFlag(
      "minComponents",
      this->minComponents, true);

    FlagSingle<int> restrictedSpeakerFlag(
      "restrictedSpeaker",
      this->restrictedSpeaker, true);

    FlagSingle<int> restrictedPhonemeFlag(
      "restrictedPhoneme",
      this->restrictedPhoneme, true);

    /////////////////////////////////////////////////////////////////////////

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&trainingDataFlag);
    parser.define_flag(&testingDataFlag);

    parser.define_flag(&outputFlag);

    // minimizer settings
    parser.define_flag(&priorSizeFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.define_flag(&restrictedSpeakerFlag);
    parser.define_flag(&restrictedPhonemeFlag);
    parser.define_flag(&minComponentsFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
