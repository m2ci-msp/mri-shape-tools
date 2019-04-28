#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <algorithm>
#include <map>
#include <string>

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagList.h"
#include "flags/FlagsParser.h"
#include "optimization/fitmodel/MinimizerSettings.h"
#include "optimization/fitmodel/EnergySettings.h"

class Settings {

public:

  // input and output
  std::string model;
  std::string inputFileName;
  std::string outputFileName;

  std::string fixedSpeakerWeights = "";
  bool fixSpeaker = false;

  std::string emaModifications;
  bool applyModifications = false;

  std::vector<std::string> channels;
  std::vector<int> sourceIds;

  fitModel::MinimizerSettings minimizerSettings;
  fitModel::EnergySettings energySettings;

  double speakerWeight = 1;
  double phonemeWeight = 1;

  double meanBiasWeight = 0;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> inputFlag("input", this->inputFileName);
    FlagSingle<std::string> modelFlag("model", this->model);
    FlagSingle<std::string> outputFlag("output", this->outputFileName);
    FlagSingle<std::string> fixedSpeakerWeightsFlag("fixedSpeakerWeights", this->fixedSpeakerWeights, true);

    // ema modifications
    FlagSingle<std::string> emaModificationsFlag("emaModifications", this->emaModifications, true);

    // channel and vertex id correspondences
    FlagList<std::string> channelsFlag("channels", this->channels);
    FlagList<int> sourceIdsFlag("sourceIds", this->sourceIds);

    // smoothness for speaker and phoneme mode
    FlagSingle<double> speakerWeightFlag("speakerWeight", this->speakerWeight, true);
    FlagSingle<double> phonemeWeightFlag("phonemeWeight", this->phonemeWeight, true);
    FlagSingle<double> meanBiasWeightFlag("meanBiasWeight", this->meanBiasWeight, true);

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
    parser.define_flag(&inputFlag);
    parser.define_flag(&modelFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&fixedSpeakerWeightsFlag);

    // channel and vertex id correspondences
    parser.define_flag(&channelsFlag);
    parser.define_flag(&sourceIdsFlag);

    // ema modifications
    parser.define_flag(&emaModificationsFlag);

    // smoothness for speaker and phoneme mode
    parser.define_flag(&speakerWeightFlag);
    parser.define_flag(&phonemeWeightFlag);

    parser.define_flag(&meanBiasWeightFlag);

    // minimizer settings
    parser.define_flag(&priorSizeFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

    // set fixed settings

    // we are not using nearest neighbor discovery -> use only one iteration
    this->minimizerSettings.iterationAmount = 1;

    // use fixed correspondences
    this->energySettings.searchStrategy =
      fitModel::EnergySettings::SearchStrategy::FIXED;

    // set weights
    this->energySettings.weights["speakerSmoothnessTerm"] = speakerWeight;
    this->energySettings.weights["phonemeSmoothnessTerm"] = phonemeWeight;
    this->energySettings.weights["meanBiasTerm"] = meanBiasWeight;

    this->fixSpeaker = fixedSpeakerWeightsFlag.is_present();

    this->applyModifications = emaModificationsFlag.is_present();

    // integrity check
    if( this->channels.size() != this->sourceIds.size()) {
      throw std::runtime_error("SourceIds and channels size mismatch!");
    }

    // make sure that sourceIds are sorted in ascending order
    // and preserve sourceId ->channel correspondence

    // build correspondence map
    std::map<int, std::string> channelMap;

    for(unsigned int i = 0; i < this->channels.size(); ++i) {
      channelMap[this->sourceIds.at(i)] = this->channels.at(i);
    }

    // sort in ascending order
    std::sort(this->sourceIds.begin(), this->sourceIds.end());

    // restore correspondences
    this->channels.clear();
    for(const int& id: this->sourceIds) {
      this->channels.push_back(channelMap[id]);
    }

  }

};
#endif
