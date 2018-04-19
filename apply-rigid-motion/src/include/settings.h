#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

class Settings {

public:

  // input and output
  std::string input;
  std::string motion;
  std::string output;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> motionFlag("motion", this->motion);
    FlagSingle<std::string> outputFlag("output", this->output);

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&inputFlag);
    parser.define_flag(&motionFlag);
    parser.define_flag(&outputFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
