#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagFixedList.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

  public:

    // input and output
    std::string inputScan;

    std::string outputScan;

    // minimum coordinate
    std::vector<int> min{0, 0, 0};

    // maximum coordinate
    std::vector<int> max{0, 0, 0};

    Settings(int argc, char* argv[]) {

      FlagSingle<std::string> inputScanFlag("input", this->inputScan);
      FlagSingle<std::string> outputScanFlag("output", this->outputScan);

      FlagFixedList<int> minFlag("min", this->min, 3);
      FlagFixedList<int> maxFlag("max", this->max, 3);

      FlagsParser parser(argv[0]);
      parser.define_flag(&inputScanFlag);
      parser.define_flag(&outputScanFlag);
      parser.define_flag(&minFlag);
      parser.define_flag(&maxFlag);

      parser.parse_from_command_line(argc, argv);
    }

};

#endif
