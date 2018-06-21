#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include "optimization/fitmodel/EnergySettings.h"
#include "optimization/fitmodel/MinimizerSettings.h"

#include <string>

class Settings {

public:

  // input and output
  std::string correspondenceFile;
  std::string emaFile;
  std::string model;

  std::string outputFile;
  std::string coilsOutputFile;

  fitModel::MinimizerSettings minimizerSettings;

  double shiftX = 0;
  double shiftY = 0;
  double shiftZ = 0;

  bool enforceMidsagittal = false;
  bool outputCoils = false;

  double scaleFactor = 1;

  int timeFrame = 0;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> emaFileFlag("emaFile", this->emaFile);
    FlagSingle<std::string> modelFlag("model", this->model);
    FlagSingle<std::string> correspondenceFileFlag("correspondenceFile", this->correspondenceFile);

    FlagSingle<std::string> outputFileFlag("outputFile", this->outputFile);
    FlagSingle<std::string> coilsOutputFileFlag("coilsOutputFile", this->coilsOutputFile, true);

    FlagSingle<int> timeFrameFlag("timeFrame", this->timeFrame);

    FlagSingle<double> scaleFactorFlag("scaleFactor", this->scaleFactor, true);

    // for shifting
    FlagSingle<double> shiftXFlag("shiftX", this->shiftX, true);
    FlagSingle<double> shiftYFlag("shiftY", this->shiftY, true);
    FlagSingle<double> shiftZFlag("shiftZ", this->shiftZ, true);

    FlagNone enforceMidsagittalFlag("enforceMidsagittal", this->enforceMidsagittal);

    // minimizer settings
    FlagSingle<double> priorSizeFlag(
      "priorSize", this->minimizerSettings.priorSize, true);

    FlagSingle<int> convergenceFactorFlag(
      "convergenceFactor",
      this->minimizerSettings.convergenceFactor, true);

    FlagSingle<double> projectedGradientToleranceFlag(
      "projectedGradientTolerance",
      this->minimizerSettings.projectedGradientTolerance, true);

    FlagSingle<int> maxFunctionEvalsFlag(
      "maxFunctionEvals",
      this->minimizerSettings.maxFunctionEvals, true);

    /////////////////////////////////////////////////////////////////////////

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&emaFileFlag);
    parser.define_flag(&modelFlag);
    parser.define_flag(&correspondenceFileFlag);
    parser.define_flag(&outputFileFlag);
    parser.define_flag(&coilsOutputFileFlag);

    // frame index
    parser.define_flag(&timeFrameFlag);

    // scaling
    parser.define_flag(&scaleFactorFlag);


    parser.define_flag(&shiftXFlag);
    parser.define_flag(&shiftYFlag);
    parser.define_flag(&shiftZFlag);

    parser.define_flag(&enforceMidsagittalFlag);

    // minimizer settings
    parser.define_flag(&priorSizeFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    parser.parse_from_command_line(argc, argv);

    // use only one iteration
    this->minimizerSettings.iterationAmount = 1;

    this->outputCoils = outputFileFlag.is_present();

  }

};
#endif
