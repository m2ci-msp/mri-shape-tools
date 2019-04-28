#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>
#include <vector>

class Settings {

public:

  // input and output
  std::string mainMesh;
  std::string coilMesh;
  std::string output;

  std::vector<int> indices;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> mainMeshFlag("mainMesh", this->mainMesh);
    FlagSingle<std::string> coilMeshFlag("coilMesh", this->coilMesh);
    FlagSingle<std::string> outputFlag("output", this->output);

    FlagList<int> indicesFlag("indices", this->indices);

    /////////////////////////////////////////////////////////////////////////

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&mainMeshFlag);
    parser.define_flag(&coilMeshFlag);
    parser.define_flag(&indicesFlag);
    parser.define_flag(&outputFlag);

    parser.parse_from_command_line(argc, argv);

  }

};
#endif
