#ifndef __MODEL_MESH_RECONSTRUCTOR_H__
#define __MODEL_MESH_RECONSTRUCTOR_H__

#include <vector>
#include <armadillo>

#include "model/ModelData.h"
#include "model/ModelReconstructor.h"

class ModelMeshReconstructor{

public:

  /*--------------------------------------------------------------------------*/

  ModelMeshReconstructor(
    const ModelData& modelData,
    const ModelReconstructor& modelReconstructor) :
    modelData(modelData), reconstruct(modelReconstructor) {
  }

  /*--------------------------------------------------------------------------*/

  Mesh for_weights(
    const arma::vec& speakerWeights,
    const arma::vec& phonemeWeights
    ) const {

    return build_mesh(
      this->reconstruct.for_weights(speakerWeights, phonemeWeights)
      );

  }

  /*--------------------------------------------------------------------------*/

  Mesh for_variations(
    const arma::vec& speakerWeights,
    const arma::vec& phonemeWeights
    ) const {

    return build_mesh(
      this->reconstruct.for_variations(speakerWeights, phonemeWeights)
      );

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  Mesh build_mesh(const arma::vec values) const {

    std::vector<arma::vec> vertices;

    for(unsigned int i = 0; i < values.n_rows; i+=3) {
      arma::vec vertex({ values(i), values(i+1), values(i+2) });
      vertices.push_back(vertex);
    }

    Mesh result = this->modelData.get_shape_space_origin_mesh();

    result.set_vertices(vertices);

    return result;
  }

  /*--------------------------------------------------------------------------*/

  const ModelData& modelData;
  const ModelReconstructor& reconstruct;

  /*--------------------------------------------------------------------------*/

};

#endif
