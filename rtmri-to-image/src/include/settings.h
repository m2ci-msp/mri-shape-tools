#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string videoFile;
  std::string outputFile;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> videoFileFlag("videoFile", this->videoFile);
    FlagSingle<std::string> outputFileFlag("outputFile", this->outputFile);

    FlagsParser parser(argv[0]);
    parser.define_flag(&videoFileFlag);
    parser.define_flag(&outputFileFlag);

    parser.parse_from_command_line(argc, argv);
  }

};

#endif
