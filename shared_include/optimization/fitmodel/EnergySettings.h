#ifndef __FIT_MODEL_ENERGY_SETTINGS_H__
#define __FIT_MODEL_ENERGY_SETTINGS_H__

#include <map>

namespace fitModel{

  class EnergySettings{

  public:

    EnergySettings() {
      this->weights["dataTerm"] = 1;
      this->weights["landmarkTerm"] = 0;
      this->weights["speakerSmoothnessTerm"] = 0;
      this->weights["phonemeSmoothnessTerm"] = 0;
      this->weights["meanBiasTerm"] = 0;
    }

    // use target as source?
    // this might be needed if for each point in the target only one corresponding
    // neighbor in the source is desired
    bool useTargetAsSource = false;

    // should source points be projected onto the normal plane of the
    // target point?
    bool useProjection = true;

    // weights for the different energy terms
    std::map<std::string, double> weights;

    // upper bound for distance during nearest neighbor search
    double maxDistance = 5;

    // upper bound for angle between normals during nearest neighbor search
    double maxAngle = 60;

    // search radius for adaptive nearest neighbor search
    double searchRadius = 5;

    enum SearchStrategy{
      BASIC,
      NORMAL_PLANE,
      ADAPTIVE,
      FIXED
    };

    // search strategy for nearest neighbor search
    SearchStrategy searchStrategy = SearchStrategy::NORMAL_PLANE;
  };

}

#endif
