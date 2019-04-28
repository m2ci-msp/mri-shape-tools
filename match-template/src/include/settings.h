#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagsParser.h"
#include "flags/FlagNone.h"

#include "optimization/matchtemplate/EnergySettings.h"
#include "optimization/matchtemplate/MinimizerSettings.h"

#include "optimization/rigidalignment/EnergySettings.h"
#include "optimization/rigidalignment/MinimizerSettings.h"


#include <string>

class Settings {

public:

  // input and output
  std::string target;
  std::string source;
  std::string output;
  std::string landmarks;

  std::string meshNeighborhood;

  bool meshNeighborhoodPresent = false;

  double geodesicNeighborhoodSize = 2;
  double sphericalNeighborhoodSize = 2;

  bool addGeodesic = false;
  bool addSpherical = false;

  matchTemplate::MinimizerSettings minimizerSettings;
  matchTemplate::EnergySettings energySettings;

  rigidAlignment::MinimizerSettings rigidAlignmentMinimizerSettings;
  rigidAlignment::EnergySettings rigidAlignmentEnergySettings;

  double smoothnessTermWeight = 1;
  double smoothnessTermWeightEnd = 1;
  double postSmoothnessTermWeight = 0;
  double smoothnessTermChange = 0;

  double landmarkTermWeight = 1;
  double landmarkTermWeightEnd = 1;
  double landmarkTermChange = 0;

  int meshSmoothIterations = 0;

  bool landmarksPresent = false;

  bool fixedNeighbors = false;
  bool useNoProjection = false;

  bool performRigidAlignment = false;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> targetFlag("target", this->target);
    FlagSingle<std::string> sourceFlag("source", this->source);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> landmarksFlag("landmarks", this->landmarks, true);
    FlagSingle<std::string> meshNeighborhoodFlag("meshNeighborhood", this->meshNeighborhood, true);

    /////////////////////////////////////////////////////////////////////////

    FlagSingle<int> meshSmoothIterationsFlag("meshSmoothIterations", this->meshSmoothIterations, true);

    /////////////////////////////////////////////////////////////////////////

    // minimizer settings
    FlagSingle<int> iterationAmountFlag(
      "iter", this->minimizerSettings.iterationAmount, true);

    FlagSingle<int> convergenceFactorFlag(
      "convergenceFactor",
      this->minimizerSettings.convergenceFactor, true);

    FlagSingle<double> projectedGradientToleranceFlag(
      "projectedGradientTolerance",
      this->minimizerSettings.projectedGradientTolerance, true);

    FlagSingle<int> maxFunctionEvalsFlag(
      "maxFunctionEvals",
      this->minimizerSettings.maxFunctionEvals, true);

    /////////////////////////////////////////////////////////////////////////

    // nearest neighbor settings
    FlagSingle<double> maxDistanceFlag(
      "maxDistance", this->energySettings.maxDistance, true);

    FlagSingle<double> maxAngleFlag(
      "maxAngle", this->energySettings.maxAngle, true);

    // set default value
    this->energySettings.searchRadius = 0;
    FlagSingle<double> searchRadiusFlag(
      "searchRadius", this->energySettings.searchRadius, true);

    FlagNone fixedNeighborsFlag("fixedNeighbors", this->fixedNeighbors);
    FlagNone useNoProjectionFlag("useNoProjection", this->useNoProjection);

    FlagSingle<double> smoothnessTermWeightFlag("smoothnessTermWeight", this->smoothnessTermWeight, true);
    FlagSingle<double> smoothnessTermWeightEndFlag("smoothnessTermWeightEnd", this->smoothnessTermWeightEnd, true);
    FlagSingle<double> postSmoothnessTermWeightFlag("postSmoothnessTermWeight", this->postSmoothnessTermWeight, true);

    FlagSingle<double> landmarkTermWeightFlag("landmarkTermWeight", this->landmarkTermWeight, true);
    FlagSingle<double> landmarkTermWeightEndFlag("landmarkTermWeightEnd", this->landmarkTermWeightEnd, true);

    FlagSingle<double> geodesicNeighborhoodSizeFlag("geodesicNeighbors", this->geodesicNeighborhoodSize, true);
    FlagSingle<double> sphericalNeighborhoodSizeFlag("sphericalNeighbors", this->sphericalNeighborhoodSize, true);

    /////////////////////////////////////////////////////////////////////////

    FlagNone performRigidAlignmentFlag("performRigidAlignment", this->performRigidAlignment);

    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&targetFlag);
    parser.define_flag(&sourceFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&landmarksFlag);
    parser.define_flag(&meshNeighborhoodFlag);

    parser.define_flag(&meshSmoothIterationsFlag);

    // energy settings
    parser.define_flag(&smoothnessTermWeightFlag);
    parser.define_flag(&smoothnessTermWeightEndFlag);
    parser.define_flag(&postSmoothnessTermWeightFlag);

    parser.define_flag(&landmarkTermWeightFlag);
    parser.define_flag(&landmarkTermWeightEndFlag);

    parser.define_flag(&geodesicNeighborhoodSizeFlag);
    parser.define_flag(&sphericalNeighborhoodSizeFlag);

    // minimizer settings
    parser.define_flag(&iterationAmountFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);

    // nearest neighbor settings
    parser.define_flag(&maxDistanceFlag);
    parser.define_flag(&maxAngleFlag);
    parser.define_flag(&searchRadiusFlag);
    parser.define_flag(&fixedNeighborsFlag);
    parser.define_flag(&useNoProjectionFlag);

    parser.define_flag(&performRigidAlignmentFlag);

    parser.parse_from_command_line(argc, argv);

    if( smoothnessTermWeightEndFlag.is_present() ) {

      const double distance = this->smoothnessTermWeight - this->smoothnessTermWeightEnd;
      this->smoothnessTermChange = distance / ( this->minimizerSettings.iterationAmount - 1 );

    }

    if( landmarkTermWeightEndFlag.is_present() ) {

      const double distance = this->landmarkTermWeight - this->landmarkTermWeightEnd;
      this->landmarkTermChange = distance / ( this->minimizerSettings.iterationAmount - 1 );

    }


    this->energySettings.useProjection = !this->useNoProjection;

    if( this->fixedNeighbors == true) {

      this->energySettings.searchStrategy =
        matchTemplate::EnergySettings::SearchStrategy::FIXED;

      this->rigidAlignmentEnergySettings.searchStrategy =
        rigidAlignment::EnergySettings::SearchStrategy::FIXED;

    }
    else if( searchRadiusFlag.is_present() == true ) {

      this->energySettings.searchStrategy =
        matchTemplate::EnergySettings::SearchStrategy::ADAPTIVE;

      this->rigidAlignmentEnergySettings.searchStrategy =
        rigidAlignment::EnergySettings::SearchStrategy::ADAPTIVE;

    }

    this->landmarksPresent = landmarksFlag.is_present();

    this->addGeodesic = geodesicNeighborhoodSizeFlag.is_present();
    this->addSpherical = sphericalNeighborhoodSizeFlag.is_present();
    this->meshNeighborhoodPresent = meshNeighborhoodFlag.is_present();

    // copy settings to rigid alignment data structures
    this->rigidAlignmentEnergySettings.useProjection = this->energySettings.useProjection;
    this->rigidAlignmentEnergySettings.maxDistance = this->energySettings.maxDistance;
    this->rigidAlignmentEnergySettings.maxAngle = this->energySettings.maxAngle;
    this->rigidAlignmentEnergySettings.searchRadius = this->energySettings.searchRadius;

    this->rigidAlignmentMinimizerSettings.iterationAmount = this->minimizerSettings.iterationAmount;
    this->rigidAlignmentMinimizerSettings.convergenceFactor = this->minimizerSettings.convergenceFactor;
    this->rigidAlignmentMinimizerSettings.projectedGradientTolerance = this->minimizerSettings.projectedGradientTolerance;
    this->rigidAlignmentMinimizerSettings.maxFunctionEvals = this->minimizerSettings.maxFunctionEvals;

  }

};

#endif
