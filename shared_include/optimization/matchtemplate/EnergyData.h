#ifndef __MATCH_TEMPLATE_ENERGY_DATA_H__
#define __MATCH_TEMPLATE_ENERGY_DATA_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "mesh/NormalEstimation.h"

#include "mesh/Mesh.h"
#include "mesh/MeshNeighbors.h"

#include "landmark/Landmark.h"

namespace matchTemplate{

  class EnergyData{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyData(
      const Mesh& source,
      const MeshNeighbors& sourceNeighbors,
      const Mesh& target
      ): source(source), sourceNeighbors(sourceNeighbors), target(target) {

      this->vertexAmount = this->source.get_vertices().size();

      this->transformations = vnl_vector<double>(
        this->vertexAmount * this->transformationAmount, 0.);

    }

    /*--------------------------------------------------------------------------*/

    // original undeformed template
    Mesh source;
    // data structure for querying vertex neighbors on the source mesh
    MeshNeighbors sourceNeighbors;

    // target
    Mesh target;

    // transformation parameters for each vertex in serialized format:
    // tx: translation in x-direction
    // ty: translation in y-direction
    // tz: translation in z-direction
    // theta: rotation along rotation axis
    // ax: x-value of rotation axis
    // ay: y-value of rotation axis
    // az: z-value of rotation axis
    vnl_vector<double> transformations;

    // transformations after template matching is complete
    vnl_vector<double> matchResult;

    std::vector<Landmark> landmarks;

    /*--------------------------------------------------------------------------*/

    const int transformationAmount = 7;
    int vertexAmount = 0;

    /*--------------------------------------------------------------------------*/

  };
}

#endif
