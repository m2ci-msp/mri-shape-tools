#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <vector>
#include <string>

class Settings {

public:

  // input and output
  std::vector<std::string> input;
  std::vector<std::string> output;

  bool noTranslation = false;
  bool noRotation = false;
  bool noScaling = false;

  int originIndex;
  bool suzanneMapping = false;
  bool changeOrigin = false;

  int iter = 10;

  Settings(int argc, char* argv[]) {

    FlagList<std::string> inputFlag("input", this->input);
    FlagList<std::string> outputFlag("output", this->output);

    FlagSingle<int> iterFlag("iter", this->iter);
    FlagNone noTranslationFlag("noTranslation", this->noTranslation);
    FlagNone noRotationFlag("noRotation", this->noRotation);
    FlagNone noScalingFlag("noScaling", this->noScaling);
    FlagSingle<int> originIndexFlag("originIndex", this->originIndex, true);
    FlagNone suzanneMappingFlag("suzanneMapping", this->suzanneMapping);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);

    parser.define_flag(&iterFlag);

    parser.define_flag(&noTranslationFlag);
    parser.define_flag(&noRotationFlag);
    parser.define_flag(&noScalingFlag);
    parser.define_flag(&originIndexFlag);
    parser.define_flag(&suzanneMappingFlag);

    parser.parse_from_command_line(argc, argv);

    this->changeOrigin = originIndexFlag.is_present();

  }

};

#endif
