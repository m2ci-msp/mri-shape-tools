#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::vector<std::string> fileList;
  std::string outputFile;

  // grid spacings of the image sequence
  double spacingX = 1.;
  double spacingY = 1.;
  double spacingT = 1.;

  Settings(int argc, char* argv[]) {

    FlagList<std::string> fileListFlag("fileList", this->fileList);
    FlagSingle<std::string> outputFileFlag("outputFile", this->outputFile);

    FlagSingle<double> spacingXFlag("spacingX", this->spacingX);
    FlagSingle<double> spacingYFlag("spacingY", this->spacingY);
    FlagSingle<double> spacingTFlag("spacingT", this->spacingT);

    FlagsParser parser(argv[0]);
    parser.define_flag(&fileListFlag);
    parser.define_flag(&outputFileFlag);

    parser.define_flag(&spacingXFlag);
    parser.define_flag(&spacingYFlag);
    parser.define_flag(&spacingTFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
