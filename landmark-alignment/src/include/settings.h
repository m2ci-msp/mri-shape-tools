#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

  public:
    bool noScaling = false;
    bool noRotation = false;
    bool noTranslation = false;

    std::string source;
    std::string landmarks;
    std::string output;

    Settings(int argc, char* argv[]) {

      FlagSingle<std::string> sourceFlag("source", this->source);
      FlagSingle<std::string> landmarksFlag("landmarks", this->landmarks);
      FlagSingle<std::string> outputFlag("output", this->output);
      FlagNone noScalingFlag ("noScaling", this->noScaling);
      FlagNone noRotationFlag ("noRotation", this->noRotation);
      FlagNone noTranslationFlag ("noTranslation", this->noTranslation);

      FlagsParser parser(argv[0]);
      parser.define_flag(&sourceFlag);
      parser.define_flag(&landmarksFlag);
      parser.define_flag(&outputFlag);
      parser.define_flag(&noScalingFlag);
      parser.define_flag(&noRotationFlag);
      parser.define_flag(&noTranslationFlag);

      parser.parse_from_command_line(argc, argv);
    }

};

#endif
