#ifndef __TENSOR_BUILDER_H__
#define __TENSOR_BUILDER_H__

#include <vector>
#include <armadillo>

#include "tensor/TensorData.h"
#include "tensor/Tensor.h"
#include "training/TrainingData.h"

class TensorBuilder{

public:

  /*----------------------------------------------------------------------*/

  static Tensor build_tensor(const TrainingData& training) {

    std::vector<double> rawData;

    for(const arma::vec& vector: training.data ) {

      for(const double& value: vector) {
        rawData.push_back(value);
      }

    }

    TensorData data;
    data.set_data(rawData)\
        .set_mode_dimensions(
          training.speakerAmount,
          training.phonemeAmount,
          training.spaceSize
          );

    return Tensor(data);
  }

  /*----------------------------------------------------------------------*/

};
#endif
