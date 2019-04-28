#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string source;
  std::string output;
  std::string outputCloud;

  std::string specifications;

  bool createCloud = false;
  bool flip = false;

  double integrationRho = 1;
  double presmoothSigma = 1;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> sourceFlag("source", this->source);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> outputCloudFlag("cloud", this->outputCloud, true);
    FlagSingle<std::string> specificationsFlag("specifications", this->specifications);
    FlagNone flipFlag("flip", this->flip);

    FlagSingle<double> presmoothSigmaFlag("presmoothSigma", this->presmoothSigma, true);
    FlagSingle<double> integrationRhoFlag("integrationRho", this->integrationRho, true);

    FlagsParser parser(argv[0]);
    parser.define_flag(&sourceFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&outputCloudFlag);
    parser.define_flag(&specificationsFlag);
    parser.define_flag(&flipFlag);

    parser.define_flag(&presmoothSigmaFlag);
    parser.define_flag(&integrationRhoFlag);

    parser.parse_from_command_line(argc, argv);

    this->createCloud = outputCloudFlag.is_present();
  }

};

#endif
