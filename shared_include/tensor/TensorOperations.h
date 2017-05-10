#ifndef __TENSOR_OPERATIONS_H__
#define __TENSOR_OPERATIONS_H__

#include <armadillo>

#include "tensor/TensorModes.h"

class TensorOperations{

public:

  /*---------------------------------------------------------------------------*/

  TensorOperations(TensorModes& tensorModes) : tensorModes(tensorModes) {
  }

  /*---------------------------------------------------------------------------*/

  TensorOperations& mode_one_multiply(const arma::mat& matrix) {

    const arma::mat mode_one_matrix = this->tensorModes.get_mode_one_matrix();

    const arma::mat result = matrix * mode_one_matrix;

    this->tensorModes.unfold_to_mode_one_vectors(result);

    return *this;

  }

  /*---------------------------------------------------------------------------*/

  TensorOperations& mode_two_multiply(const arma::mat& matrix) {

    const arma::mat mode_two_matrix = this->tensorModes.get_mode_two_matrix();

    const arma::mat result = matrix * mode_two_matrix;

    this->tensorModes.unfold_to_mode_two_vectors(result);

    return *this;

  }

  /*---------------------------------------------------------------------------*/

  TensorOperations& mode_three_multiply(const arma::mat& matrix) {

    const arma::mat mode_three_matrix = this->tensorModes.get_mode_three_matrix();

    const arma::mat result = matrix * mode_three_matrix;

    this->tensorModes.unfold_to_mode_three_vectors(result);

    return *this;

  }

  /*---------------------------------------------------------------------------*/


private:

  /*---------------------------------------------------------------------------*/

  TensorModes& tensorModes;

  /*---------------------------------------------------------------------------*/

};

#endif
