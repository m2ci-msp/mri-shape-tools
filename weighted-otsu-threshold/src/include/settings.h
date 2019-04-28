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

  double weightInside = 1;
  double weightOutside = 1;

  bool createCloud = false;
  bool flip = false;

  double integrationRho = 1;
  double presmoothSigma = 1;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> outputCloudFlag("cloud", this->outputCloud, true);
    FlagNone flipFlag("flip", this->flip);

    FlagSingle<double> weightInsideFlag("weightInside", this->weightInside, true);
    FlagSingle<double> weightOutsideFlag("weightOutside", this->weightOutside, true);

    FlagSingle<double> presmoothSigmaFlag("presmoothSigma", this->presmoothSigma, true);
    FlagSingle<double> integrationRhoFlag("integrationRho", this->integrationRho, true);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&outputCloudFlag);
    parser.define_flag(&flipFlag);

    parser.define_flag(&weightInsideFlag);
    parser.define_flag(&weightOutsideFlag);

    parser.define_flag(&presmoothSigmaFlag);
    parser.define_flag(&integrationRhoFlag);

    parser.parse_from_command_line(argc, argv);

    this->createCloud = outputCloudFlag.is_present();
  }

};

#endif
