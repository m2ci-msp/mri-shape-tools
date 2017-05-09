#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string source;
  std::string target;
  std::string landmarks;
  std::string output;

  bool changePresent = false;
  bool noTranslation = false;
  bool noRotation = false;
  bool noScaling = false;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> sourceFlag("source", this->source);
    FlagSingle<std::string> targetFlag("target", this->target);
    FlagSingle<std::string> landmarksFlag("landmarks", this->landmarks);
    FlagSingle<std::string> outputFlag("output", this->output);

    FlagNone noTranslationFlag("noTranslation", this->noTranslation);
    FlagNone noRotationFlag("noRotation", this->noRotation);
    FlagNone noScalingFlag("noScaling", this->noScaling);

    FlagsParser parser(argv[0]);

    parser.define_flag(&sourceFlag);
    parser.define_flag(&landmarksFlag);
    parser.define_flag(&targetFlag);
    parser.define_flag(&outputFlag);

    parser.define_flag(&noTranslationFlag);
    parser.define_flag(&noRotationFlag);
    parser.define_flag(&noScalingFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
