#ifndef __RIGID_ALIGNMENT_ENERGY_DATA_H__
#define __RIGID_ALIGNMENT_ENERGY_DATA_H__

#include <vector>

#include <armadillo>
#include <vnl/vnl_vector.h>

#include "landmark/Landmark.h"
#include "mesh/Mesh.h"
#include "optimization/rigidalignment/Transformation.h"
#include "optimization/rigidalignment/TransformationEnum.h"

namespace rigidAlignment{

  class EnergyData{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyData(const Mesh& source, const Mesh& target): source(source), target(target) {

      this->transformation = vnl_vector<double>(this->transformationAmount, 0.);

    }

    /*--------------------------------------------------------------------------*/

    // original undeformed template
    Mesh source;

    // target
    Mesh target;

    // transformation parameters in serialized format:
    vnl_vector<double> transformation;

    // transformation matrix
    Transformation transformationMatrix;

    std::vector<Landmark> landmarks;

    /*--------------------------------------------------------------------------*/

    const int transformationAmount = 7;

    /*--------------------------------------------------------------------------*/

  };
}

#endif
