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
  bool changeOrigin = false;

  int iter = 10;

  std::string mapping = "XYZ";

  double xFactor = 1;
  double yFactor = 1;
  double zFactor = 1;

  Settings(int argc, char* argv[]) {

    FlagList<std::string> inputFlag("input", this->input);
    FlagList<std::string> outputFlag("output", this->output);

    FlagSingle<int> iterFlag("iter", this->iter);
    FlagNone noTranslationFlag("noTranslation", this->noTranslation);
    FlagNone noRotationFlag("noRotation", this->noRotation);
    FlagNone noScalingFlag("noScaling", this->noScaling);
    FlagSingle<int> originIndexFlag("originIndex", this->originIndex, true);

    FlagSingle<std::string> mappingFlag("mapping", this->mapping, true);
    FlagSingle<double> xFactorFlag("xFactor", this->xFactor, true);
    FlagSingle<double> yFactorFlag("yFactor", this->yFactor, true);
    FlagSingle<double> zFactorFlag("zFactor", this->zFactor, true);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);

    parser.define_flag(&iterFlag);

    parser.define_flag(&noTranslationFlag);
    parser.define_flag(&noRotationFlag);
    parser.define_flag(&noScalingFlag);
    parser.define_flag(&originIndexFlag);

    parser.define_flag(&mappingFlag);

    parser.define_flag(&xFactorFlag);
    parser.define_flag(&yFactorFlag);
    parser.define_flag(&zFactorFlag);

    parser.parse_from_command_line(argc, argv);

    this->changeOrigin = originIndexFlag.is_present();

  }

};

#endif
