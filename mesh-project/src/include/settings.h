#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

  public:

    std::string mesh;
    std::string scan;
    std::string output;

    Settings(int argc, char* argv[]) {

      FlagSingle<std::string> meshFlag("mesh", this->mesh);
      FlagSingle<std::string> scanFlag("scan", this->scan);
      FlagSingle<std::string> outputFlag("output", this->output);

      FlagsParser parser(argv[0]);
      parser.define_flag(&meshFlag);
      parser.define_flag(&scanFlag);
      parser.define_flag(&outputFlag);

      parser.parse_from_command_line(argc, argv);

    }

};

#endif
