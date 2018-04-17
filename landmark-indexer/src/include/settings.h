#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

  public:

    std::string sourceMesh;
    std::string landmarks;
    std::string output;

    Settings(int argc, char* argv[]) {

      FlagSingle<std::string> sourceMeshFlag("sourceMesh", this->sourceMesh);
      FlagSingle<std::string> landmarksFlag("landmarks", this->landmarks);
      FlagSingle<std::string> outputFlag("output", this->output);

      FlagsParser parser(argv[0]);
      parser.define_flag(&sourceMeshFlag);
      parser.define_flag(&landmarksFlag);
      parser.define_flag(&outputFlag);

      parser.parse_from_command_line(argc, argv);

    }

};

#endif
