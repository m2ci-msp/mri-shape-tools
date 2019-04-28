#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

class Settings {

public:

  std::string input;

  std::string axisAccess = "XYZ";

  double spacing = 1;

  double lambda = 0;

  std::string output;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> axisAccessFlag("axisAccess", this->axisAccess, true);

    FlagSingle<double> spacingFlag("spacing", this->spacing, true);

    FlagSingle<double> lambdaFlag("lambda", this->lambda, true);

    FlagSingle<std::string> outputFlag("output", this->output);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&axisAccessFlag);

    parser.define_flag(&spacingFlag);

    parser.define_flag(&lambdaFlag);

    parser.define_flag(&outputFlag);

    parser.parse_from_command_line(argc, argv);

  }

};
#endif
