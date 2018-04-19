#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string inputScan;

  std::string outputScan;

  int timeSteps = 1;
  double stepSize = 0.125;
  double contrastLambda = 1;
  double integrationRho = 1;
  double presmoothSigma = 1;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputScanFlag("input", this->inputScan);
    FlagSingle<std::string> outputScanFlag("output", this->outputScan);

    FlagSingle<double> sigmaFlag("sigma", this->presmoothSigma, true);
    FlagSingle<double> rhoFlag("rho", this->integrationRho, true);
    FlagSingle<double> lambdaFlag("lambda", this->contrastLambda, true);
    FlagSingle<int> iterFlag("iter", this->timeSteps, true);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputScanFlag);
    parser.define_flag(&outputScanFlag);

    parser.define_flag(&sigmaFlag);
    parser.define_flag(&rhoFlag);
    parser.define_flag(&lambdaFlag);
    parser.define_flag(&iterFlag);

    parser.parse_from_command_line(argc, argv);
  }

};

#endif
