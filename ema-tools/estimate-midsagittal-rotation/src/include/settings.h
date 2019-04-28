#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::vector<std::string> input;
  std::string output;

  std::vector<std::string> midsagittalCoils;

  Settings(int argc, char* argv[]) {

    FlagList<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagList<std::string> midsagittalFlag("midsagittalCoils", this->midsagittalCoils);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&midsagittalFlag);

    parser.parse_from_command_line(argc, argv);
  }

};

#endif
