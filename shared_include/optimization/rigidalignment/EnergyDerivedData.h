#ifndef __RIGID_ALIGNMENT_ENERGY_DERIVED_DATA__
#define __RIGID_ALIGNMENT_ENERGY_DERIVED_DATA__

#include <vector>

#include <armadillo>

#include "mesh/Mesh.h"

namespace rigidAlignment{

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

    /* linearized vertex data of source and target mesh
     *
     * only vertices belonging to the neighbor correspondences are set,
     * all other values are 0
     *
     * ATTENTION: linearizedTarget may contain points that were projected
     * onto the normal plane of the corresponding target vertex
     */
    arma::vec linearizedSource;
    arma::vec linearizedTarget;

    /* linearized data for the landmark correspondences */
    arma::vec linearizedLandmarkSource;
    arma::vec linearizedLandmarkTarget;

    /* indicators for landmark presence */
    std::vector<bool> isLandmark;

    /* weights derived from settings and current data */
    std::map<std::string, double> weights;

    /*--------------------------------------------------------------------------*/

  };
}
#endif
