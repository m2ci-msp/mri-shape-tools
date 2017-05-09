#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"
#include "flags/FlagList.h"
#include "flags/FlagNone.h"

#include <string>

class Settings {

public:

  std::string input;
  std::string baseName;

  std::vector<int> xySlices;
  std::vector<int> xzSlices;
  std::vector<int> yzSlices;

  bool xyOutput;
  bool xzOutput;
  bool yzOutput;

  bool discard = false;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> outputFlag("output", this->baseName);
    FlagList<int> xySlicesFlag("xy", this->xySlices, true);
    FlagList<int> xzSlicesFlag("xz", this->xzSlices, true);
    FlagList<int> yzSlicesFlag("yz", this->yzSlices, true);
    FlagNone discardFlag("discard", this->discard);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&xySlicesFlag);
    parser.define_flag(&xzSlicesFlag);
    parser.define_flag(&yzSlicesFlag);
    parser.define_flag(&discardFlag);

    parser.parse_from_command_line(argc, argv);

    this->xyOutput = xySlicesFlag.is_set();
    this->xzOutput = xzSlicesFlag.is_set();
    this->yzOutput = yzSlicesFlag.is_set();

  }

};

#endif
