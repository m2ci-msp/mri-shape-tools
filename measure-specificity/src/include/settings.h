#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagsParser.h"
#include "flags/FlagNone.h"

#include <string>

class Settings {

public:

  // input and output
  std::string input;
  std::string output;
  int sampleAmount = 1000;

  std::vector<std::string> subsets;
  bool subsetsPresent = false;

  int truncatedSpeakerMode = 1;
  int truncatedPoseMode = 1;

  bool deactivateSpeaker = false;
  bool deactivatePhoneme = false;
  bool evaluateFixedPhoneme = false;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<int> sampleAmountFlag("sampleAmount", this->sampleAmount, true);
    FlagSingle<int> truncatedSpeakerModeFlag("truncatedSpeaker", this->truncatedSpeakerMode);
    FlagSingle<int> truncatedPoseModeFlag("truncatedPose", this->truncatedPoseMode);
    FlagList<std::string> subsetsFlag("subsets", this->subsets, true);

    FlagNone disableSpeakerFlag("disableSpeaker", this->deactivateSpeaker);
    FlagNone disablePhonemeFlag("disablePhoneme", this->deactivatePhoneme);
    FlagNone evaluateFixedPhonemeFlag("evaluateFixedPhoneme", this->evaluateFixedPhoneme);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&sampleAmountFlag);
    parser.define_flag(&truncatedSpeakerModeFlag);
    parser.define_flag(&truncatedPoseModeFlag);
    parser.define_flag(&subsetsFlag);

    parser.define_flag(&disableSpeakerFlag);
    parser.define_flag(&disablePhonemeFlag);
    parser.define_flag(&evaluateFixedPhonemeFlag);

    parser.parse_from_command_line(argc, argv);

    this->subsetsPresent = subsetsFlag.is_present();

  }

};

#endif
