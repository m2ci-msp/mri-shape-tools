#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string first;
  std::string second;
  std::string output;

  bool onlyPoints = false;
  bool computeNormals = false;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> firstFlag("first", this->first);
    FlagSingle<std::string> secondFlag("second", this->second);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagNone onlyPointsFlag("onlyPoints", this->onlyPoints);
    FlagNone computeNormalsFlag("computeNormals", this->computeNormals);

    FlagsParser parser(argv[0]);
    parser.define_flag(&firstFlag);
    parser.define_flag(&secondFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&onlyPointsFlag);
    parser.define_flag(&computeNormalsFlag);

    parser.parse_from_command_line(argc, argv);
  }

};

#endif
