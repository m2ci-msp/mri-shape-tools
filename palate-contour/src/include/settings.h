#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagsParser.h"

class Settings {

public:

  std::vector<std::string> input;
  std::vector<std::string> channels;

  std::string axisAccess = "XYZ";

  int samplesX = 100;
  int samplesY = 1000;

  double radius = 1;
  double scale = 1;
  std::string output;

  Settings(int argc, char* argv[]) {

    FlagList<std::string> inputFlag("input", this->input);
    FlagList<std::string> channelsFlag("channels", this->channels);

    FlagSingle<double> radiusFlag("radius", this->radius, true);
    FlagSingle<double> scaleFlag("scale", this->scale, true);

    FlagSingle<int> samplesXFlag("samplesX", this->samplesX, true);
    FlagSingle<int> samplesYFlag("samplesY", this->samplesY, true);

    FlagSingle<std::string> axisAccessFlag("axisAccess", this->axisAccess, true);

    FlagSingle<std::string> outputFlag("output", this->output);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&channelsFlag);
    parser.define_flag(&radiusFlag);

    parser.define_flag(&samplesXFlag);
    parser.define_flag(&samplesYFlag);

    parser.define_flag(&scaleFlag);
    parser.define_flag(&axisAccessFlag);

    parser.define_flag(&outputFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
