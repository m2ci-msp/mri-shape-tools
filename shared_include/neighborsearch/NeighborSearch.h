#ifndef __NEIGHBOR_SEARCH_H__
#define __NEIGHBOR_SEARCH_H__

#include "alignment/KdTree.h"
#include "mesh/Mesh.h"

#include "neighborsearch/BasicSearch.h"
#include "neighborsearch/NormalPlaneSearch.h"
#include "neighborsearch/AdaptiveSearch.h"
#include "neighborsearch/FixedCorrespondences.h"
#include "neighborsearch/PresetCorrespondences.h"

class NeighborSearch{

public:

  /*--------------------------------------------------------------------------*/

  NeighborSearch() {

    this->kdTree = nullptr;

    this->searchRadius = 4;
    this->maxDist = 5;
    this->maxAngle = 60;

    this->basicSearch = new BasicSearch(
      this->source,
      this->target,
      this->kdTree,
      this->maxDist
      );

    this->normalPlaneSearch = new NormalPlaneSearch(
      this->source,
      this->target,
      this->kdTree,
      this->maxDist,
      this->maxAngle
      );

    this->adaptiveSearch = new AdaptiveSearch(
      this->source,
      this->target,
      this->kdTree,
      this->maxDist,
      this->maxAngle,
      this->searchRadius
      );

    this->fixedCorrespondences = new FixedCorrespondences(
      this->source,
      this->target
      );

    this->presetCorrespondences = new PresetCorrespondences(
      this->presetSource,
      this->presetTarget
      );

    this->valid = true;

  }

  /*--------------------------------------------------------------------------*/

  NeighborSearch(const NeighborSearch& search) :
    maxDist(search.maxDist),
    maxAngle(search.maxAngle),
    searchRadius(search.searchRadius),
    source(search.source),
    target(search.target),
    presetSource(search.presetSource),
    presetTarget(search.presetTarget) {

    cleanup();

    this->basicSearch = new BasicSearch(
      this->source,
      this->target,
      this->kdTree,
      this->maxDist
      );

    this->normalPlaneSearch = new NormalPlaneSearch(
      this->source,
      this->target,
      this->kdTree,
      this->maxDist,
      this->maxAngle
      );

    this->adaptiveSearch = new AdaptiveSearch(
      this->source,
      this->target,
      this->kdTree,
      this->maxDist,
      this->maxAngle,
      this->searchRadius
      );

    this->fixedCorrespondences = new FixedCorrespondences(
      this->source,
      this->target
      );

    this->presetCorrespondences = new PresetCorrespondences(
      this->presetSource,
      this->presetTarget
      );


    if( this->target.get_vertices().size() > 0) {
      this->kdTree = new KdTree(target.get_vertices());
    }

  }

  /*--------------------------------------------------------------------------*/

  ~NeighborSearch() {

    cleanup();

  }

  /*----------------------------------------------------------------------*/

  void cleanup() {

    if( this->valid == true ) {

      delete this->basicSearch;
      delete this->normalPlaneSearch;
      delete this->adaptiveSearch;
      delete this->fixedCorrespondences;
      delete this->presetCorrespondences;


      if( this->kdTree != nullptr) {
        delete this->kdTree;
      }

    }

    this->valid = false;

  }

  /*--------------------------------------------------------------------------*/

  NeighborSearch& set_search_radius(const double& radius) {

    this->searchRadius = radius;

    return *this;
  }

  /*--------------------------------------------------------------------------*/

  NeighborSearch& set_source(const Mesh& source) {

    this->source = source;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  NeighborSearch& set_target(const Mesh& target, bool computeKdTree = true) {

    this->target = target;

    if(computeKdTree == true) {

      if(this->kdTree != nullptr) {
        delete this->kdTree;
      }

      this->kdTree = new KdTree(target.get_vertices());

    }

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  NeighborSearch& set_max_distance(const double& maxDist) {

    this->maxDist = maxDist;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  NeighborSearch& set_max_angle(const double& maxAngle) {

    this->maxAngle = maxAngle;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  NeighborSearch& set_preset_source(const std::vector<int>& presetSource) {

    this->presetSource = presetSource;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  NeighborSearch& set_preset_target(const std::vector<int>& presetTarget) {

    this->presetTarget = presetTarget;

    return *this;

  }

  /*--------------------------------------------------------------------------*/


  const BasicSearch& basic() const {

    return *this->basicSearch;

  }

  /*--------------------------------------------------------------------------*/

  const AdaptiveSearch& adaptive() const {

    return *this->adaptiveSearch;

  }

  /*--------------------------------------------------------------------------*/

  const NormalPlaneSearch& normal_plane() const {

    return *this->normalPlaneSearch;

  }

  /*--------------------------------------------------------------------------*/

  const FixedCorrespondences& fixed_correspondences() const {

    return *this->fixedCorrespondences;
  }

  /*--------------------------------------------------------------------------*/

  const PresetCorrespondences& preset_correspondences() const {

    return *this->presetCorrespondences;

  }

  /*--------------------------------------------------------------------------*/


private:

  /*--------------------------------------------------------------------------*/

  bool valid = false;

  double maxDist;
  double maxAngle;
  double searchRadius;

  Mesh source;
  Mesh target;

  // for preset neighbors
  std::vector<int> presetSource;
  std::vector<int> presetTarget;

  KdTree* kdTree;

  BasicSearch* basicSearch;
  NormalPlaneSearch* normalPlaneSearch;
  AdaptiveSearch* adaptiveSearch;
  FixedCorrespondences* fixedCorrespondences;
  PresetCorrespondences* presetCorrespondences;

  /*--------------------------------------------------------------------------*/

};

#endif
