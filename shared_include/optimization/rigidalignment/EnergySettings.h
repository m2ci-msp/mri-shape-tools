#ifndef __RIGID_ALIGNMENT_ENERGY_SETTINGS_H__
#define __RIGID_ALIGNMENT_ENERGY_SETTINGS_H__

#include <map>

namespace rigidAlignment{

  class EnergySettings{

  public:

    EnergySettings() {
      this->weights["dataTerm"] = 1;
    }

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
