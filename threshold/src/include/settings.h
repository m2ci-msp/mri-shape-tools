#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string input;
  std::string output;

  double threshold;

  bool scaleTo8Bit = false;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<double> thresholdFlag("threshold", this->threshold);
    FlagNone scaleFlag("scale_to_8bit", this->scaleTo8Bit);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&thresholdFlag);
    parser.define_flag(&scaleFlag);

    parser.parse_from_command_line(argc, argv);
  }

};

#endif
