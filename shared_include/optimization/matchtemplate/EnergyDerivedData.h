#ifndef __MATCH_TEMPLATE_ENERGY_DERIVED_DATA__
#define __MATCH_TEMPLATE_ENERGY_DERIVED_DATA__

#include <vector>

#include <armadillo>

#include "mesh/Mesh.h"
#include "optimization/matchtemplate/Transformation.h"

namespace matchTemplate{

  class EnergyDerivedData{

  public:

    /*--------------------------------------------------------------------------*/

    // current deformed template mesh
    Mesh source;

    /* indices that represent the neighbor correspondence between
     * source and target for the data term
     */
    std::vector<int> sourceIndices;
    std::vector<int> targetIndices;

    // computed neighbors that may be projections on the normal plane at some
    // point in the target
    std::vector<arma::vec> neighbors;
    // indicators if neighbor is present or not
    std::vector<int> neighborIndicator;

    // current transformations
    std::vector<Transformation> transformations;

    /* weights derived from settings and current data */
    std::map<std::string, double> weights;

    /*--------------------------------------------------------------------------*/

  };
}
#endif
