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
  std::string outputCloud;

  double threshold;

  bool scaleTo8Bit = false;

  bool createCloud = false;
  bool flip = false;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> outputCloudFlag("cloud", this->outputCloud, true);
    FlagSingle<double> thresholdFlag("threshold", this->threshold);
    FlagNone scaleFlag("scale_to_8bit", this->scaleTo8Bit);
    FlagNone flipFlag("flip", this->flip);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&outputCloudFlag);
    parser.define_flag(&thresholdFlag);
    parser.define_flag(&scaleFlag);
    parser.define_flag(&flipFlag);

    parser.parse_from_command_line(argc, argv);

    this->createCloud = outputCloudFlag.is_present();

  }

};

#endif
