#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  std::string input;
  std::string tongueModel;
  std::string outputBase;

  std::string globalTransformation;
  std::string headMotionEma;

  double startTime;
  double endTime;
  double samplingRate;
  double scaleFactor;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> tongueModelFlag("tongueModel", this->tongueModel);
    FlagSingle<std::string> outputBaseFlag("outputBase", this->outputBase);
    FlagSingle<std::string> globalTransformationFlag("globalTransformation", this->globalTransformation);
    FlagSingle<std::string> headMotionEmaFlag("headMotionEma", this->headMotionEma);

    FlagSingle<double> startTimeFlag("startTime", this->startTime);
    FlagSingle<double> endTimeFlag("endTime", this->endTime);
    FlagSingle<double> samplingRateFlag("samplingRate", this->samplingRate);
    FlagSingle<double> scaleFactorFlag("scaleFactor", this->scaleFactor);

    FlagsParser parser(argv[0]);

    parser.define_flag(&inputFlag);
    parser.define_flag(&tongueModelFlag);
    parser.define_flag(&outputBaseFlag);
    parser.define_flag(&globalTransformationFlag);
    parser.define_flag(&headMotionEmaFlag);
    parser.define_flag(&startTimeFlag);
    parser.define_flag(&endTimeFlag);
    parser.define_flag(&samplingRateFlag);
    parser.define_flag(&scaleFactorFlag);

    parser.parse_from_command_line(argc, argv);

  }

};
#endif
