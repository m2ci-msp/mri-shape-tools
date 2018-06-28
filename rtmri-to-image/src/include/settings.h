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

  Settings(int argc, char* argv[]) {

    FlagList<std::string> fileListFlag("fileList", this->fileList);
    FlagSingle<std::string> outputFileFlag("outputFile", this->outputFile);

    FlagsParser parser(argv[0]);
    parser.define_flag(&fileListFlag);
    parser.define_flag(&outputFileFlag);

    parser.parse_from_command_line(argc, argv);
  }

};

#endif
