#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

  public:

    // input and output
    std::string model;
    std::string outputBaseName;

    Settings(int argc, char* argv[]) {

      FlagSingle<std::string> modelFlag("model", this->model);
      FlagSingle<std::string> outputBaseNameFlag("outputBaseName", this->outputBaseName);

      FlagsParser parser(argv[0]);
      parser.define_flag(&modelFlag);
      parser.define_flag(&outputBaseNameFlag);

      parser.parse_from_command_line(argc, argv);

    }

};

#endif
