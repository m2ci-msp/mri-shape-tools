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
  std::string leftCoil;
  std::string rightCoil;
  std::string frontCoil;

  bool frontIsBack = false;

  int timeFrame;
  std::string output;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> leftFlag("left", this->leftCoil);
    FlagSingle<std::string> rightFlag("right", this->rightCoil);
    FlagSingle<std::string> frontFlag("front", this->frontCoil);
    FlagSingle<int> timeFrameFlag("timeFrame", this->timeFrame);
    FlagNone frontIsBackFlag("frontIsBack", this->frontIsBack);

    FlagSingle<std::string> outputFlag("output", this->output);

    FlagsParser parser(argv[0]);

    parser.define_flag(&inputFlag);
    parser.define_flag(&leftFlag);
    parser.define_flag(&rightFlag);
    parser.define_flag(&frontFlag);
    parser.define_flag(&timeFrameFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&frontIsBackFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
