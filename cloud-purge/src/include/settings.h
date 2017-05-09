#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

  public:

    std::string source;
    std::string landmarks;
    std::string boundaryMesh;
    std::string output;

  bool boundaryMeshPresent;
  bool landmarksPresent;

  bool keepPointsBelow = false;

  bool bounceSides = false;

  double sideDistance = 3;
  double maxDistance = 1;

    Settings(int argc, char* argv[]) {

      FlagSingle<std::string> sourceFlag("source", this->source);
      FlagSingle<std::string> landmarksFlag("landmarks", this->landmarks, true);
      FlagNone keepPointsBelowFlag("keepPointsBelow", this->keepPointsBelow);
      FlagNone bounceSidesFlag("bounceSides", this->bounceSides);
      FlagSingle<double> sideDistanceFlag("sideDistance", this->sideDistance, true);
      FlagSingle<double> maxDistanceFlag("maxDistance", this->maxDistance, true);
      FlagSingle<std::string> boundaryMeshFlag("boundaryMesh", this->boundaryMesh, true);
      FlagSingle<std::string> outputFlag("output", this->output);

      FlagsParser parser(argv[0]);
      parser.define_flag(&sourceFlag);
      parser.define_flag(&landmarksFlag);
      parser.define_flag(&boundaryMeshFlag);
      parser.define_flag(&keepPointsBelowFlag);
      parser.define_flag(&maxDistanceFlag);
      parser.define_flag(&outputFlag);
      parser.define_flag(&bounceSidesFlag);
      parser.define_flag(&sideDistanceFlag);

      parser.parse_from_command_line(argc, argv);

      this->boundaryMeshPresent = boundaryMeshFlag.is_present();
      this->landmarksPresent = landmarksFlag.is_present();
    }

};

#endif
