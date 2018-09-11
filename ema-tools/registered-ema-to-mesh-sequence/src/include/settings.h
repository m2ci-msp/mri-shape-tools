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

  std::string coilMesh;

  double startTime = 0;
  double endTime = 0;
  double samplingRate;

  bool startTimeSet = false;
  bool endTimeSet = false;

  bool basic = false;

  bool showCoils = false;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> tongueModelFlag("tongueModel", this->tongueModel);
    FlagSingle<std::string> outputBaseFlag("outputBase", this->outputBase);
    FlagSingle<std::string> globalTransformationFlag("globalTransformation", this->globalTransformation, true);
    FlagSingle<std::string> headMotionEmaFlag("headMotionEma", this->headMotionEma, true);
    FlagSingle<std::string> coilMeshFlag("coilMesh", this->coilMesh, true);

    FlagSingle<double> startTimeFlag("startTime", this->startTime, true);
    FlagSingle<double> endTimeFlag("endTime", this->endTime, true);
    FlagSingle<double> samplingRateFlag("samplingRate", this->samplingRate);

    FlagsParser parser(argv[0]);

    parser.define_flag(&inputFlag);
    parser.define_flag(&tongueModelFlag);
    parser.define_flag(&outputBaseFlag);
    parser.define_flag(&globalTransformationFlag);
    parser.define_flag(&headMotionEmaFlag);
    parser.define_flag(&coilMeshFlag);
    parser.define_flag(&startTimeFlag);
    parser.define_flag(&endTimeFlag);
    parser.define_flag(&samplingRateFlag);

    parser.parse_from_command_line(argc, argv);

    this->basic =

      ( headMotionEmaFlag.is_present() == false ) ||
      ( globalTransformationFlag.is_present() == false ) ||
      ( startTimeFlag.is_present() == false ) ||
      ( endTimeFlag.is_present() == false );

    this->showCoils = coilMeshFlag.is_present();

    this->startTimeSet = startTimeFlag.is_present();
    this->endTimeSet = endTimeFlag.is_present();

  }

};
#endif
