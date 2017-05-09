#ifndef __COMPACTNESS_H__
#define __COMPACTNESS_H__

#include <vector>
#include <armadillo>

#include "tensor/Tensor.h"

class Compactness{

public:

  /*---------------------------------------------------------------------------*/

  Compactness(const Tensor& tensor) : tensor(tensor) {
  }

  /*---------------------------------------------------------------------------*/

  std::vector<double> for_speaker_mode() {

    const arma::mat speakerMode = this->tensor.modes().get_mode_one_matrix();
    const double factor = 1. /
      this->tensor.data().get_mode_one_dimension();
    const arma::mat covariance = factor * speakerMode * speakerMode.t();

    return compute_compactness(covariance);

  }

  /*---------------------------------------------------------------------------*/

  std::vector<double> for_phoneme_mode() {

    const arma::mat phonemeMode = this->tensor.modes().get_mode_two_matrix();
    const double factor = 1. /
      this->tensor.data().get_mode_two_dimension();
    const arma::mat covariance = factor * phonemeMode * phonemeMode.t();

    return compute_compactness(covariance);

  }

  /*---------------------------------------------------------------------------*/

private:

  /*---------------------------------------------------------------------------*/

  std::vector<double> compute_compactness(const arma::mat& matrix) {

    std::vector<double> result;

    arma::vec eigval;

    arma::eig_sym(eigval, matrix);

    double totalSum = 0;
    eigval = arma::flipud(eigval);

    for(const double& value: eigval) {
      totalSum += value;
    }

    double sum = 0;

    for(const double& value: eigval) {
      sum += value;

      result.push_back(sum / totalSum);

    }

    return result;

  }

  /*---------------------------------------------------------------------------*/

  Tensor tensor;

  /*---------------------------------------------------------------------------*/

};

#endif
