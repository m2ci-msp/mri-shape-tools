#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"
#include "flags/FlagList.h"

#include <string>

class Settings {

public:

  std::string input;
  std::string output;

  std::string globalTransformation;
  std::string headMotionEma;

  double startTime = 0;
  double endTime = 0;
  double samplingRate;

  bool startTimeSet = false;
  bool endTimeSet = false;

  bool basic = false;

  std::vector<std::string> coilIds;

  std::string customTransformation;
  bool customTransformationSet = false;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> globalTransformationFlag("globalTransformation", this->globalTransformation, true);
    FlagSingle<std::string> headMotionEmaFlag("headMotionEma", this->headMotionEma, true);
    FlagSingle<std::string> customTransformationFlag("customTransformation", this->customTransformation, true);

    FlagSingle<double> startTimeFlag("startTime", this->startTime, true);
    FlagSingle<double> endTimeFlag("endTime", this->endTime, true);
    FlagSingle<double> samplingRateFlag("samplingRate", this->samplingRate);
    FlagList<std::string> coilIdsFlag("coilIds", this->coilIds);

    FlagsParser parser(argv[0]);

    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&globalTransformationFlag);
    parser.define_flag(&headMotionEmaFlag);
    parser.define_flag(&startTimeFlag);
    parser.define_flag(&endTimeFlag);
    parser.define_flag(&samplingRateFlag);
    parser.define_flag(&customTransformationFlag);
    parser.define_flag(&coilIdsFlag);

    parser.parse_from_command_line(argc, argv);

    this->basic =

      ( headMotionEmaFlag.is_present() == false ) ||
      ( globalTransformationFlag.is_present() == false ) ||
      ( startTimeFlag.is_present() == false ) ||
      ( endTimeFlag.is_present() == false );

    this->startTimeSet = startTimeFlag.is_present();
    this->endTimeSet = endTimeFlag.is_present();
    this->customTransformationSet = customTransformationFlag.is_present();

  }

};
#endif
