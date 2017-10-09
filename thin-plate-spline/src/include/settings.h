#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

class Settings {

public:

  std::string input;

  std::string axisAccess = "XYZ";

  int samplesX = 100;
  int samplesY = 100;

  double lambda = 0;

  std::string output;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> axisAccessFlag("axisAccess", this->axisAccess, true);

    FlagSingle<int> samplesXFlag("samplesX", this->samplesX, true);
    FlagSingle<int> samplesYFlag("samplesY", this->samplesY, true);

    FlagSingle<double> lambdaFlag("lambda", this->lambda, true);

    FlagSingle<std::string> outputFlag("output", this->output);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&axisAccessFlag);

    parser.define_flag(&samplesXFlag);
    parser.define_flag(&samplesYFlag);

    parser.define_flag(&lambdaFlag);

    parser.define_flag(&outputFlag);

    parser.parse_from_command_line(argc, argv);

  }

};
#endif
