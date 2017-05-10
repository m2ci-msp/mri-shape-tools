#ifndef __FIT_MODEL_ENERGY_DERIVED_DATA_H__
#define __FIT_MODEL_ENERGY_DERIVED_DATA_H__

#include <vector>
#include <map>

#include <armadillo>

#include "mesh/Mesh.h"

namespace fitModel{

/* class that represents data derived from the main data in EnergyData
 *
 * values of this class are set by EnergyDerivedDataUpdate.h
 */
  class EnergyDerivedData{

  public:

    /*--------------------------------------------------------------------------*/

    /* source mesh that depends on the chosen model parameters
     * selected in EnergyData
     */
    Mesh source;

    /* indices that represent the neighbor correspondence between
     * source and target
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

    /* weights derived from settings and current data */
    std::map<std::string, double> weights;

    /*--------------------------------------------------------------------------*/

  };
}
#endif
