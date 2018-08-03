#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::vector<std::string> images;
  std::vector<std::string> landmarks;

  std::string output;

  bool scaleTo8Bit = false;

  Settings(int argc, char* argv[]) {

    FlagList<std::string> imagesFlag("images", this->images);
    FlagList<std::string> landmarksFlag("landmarks", this->landmarks);
    FlagSingle<std::string> outputFlag("output", this->output);

    FlagNone scaleFlag("scale_to_8bit", this->scaleTo8Bit);

    FlagsParser parser(argv[0]);
    parser.define_flag(&imagesFlag);
    parser.define_flag(&landmarksFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&scaleFlag);

    parser.parse_from_command_line(argc, argv);

  }

};

#endif
