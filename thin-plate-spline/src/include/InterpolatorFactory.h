#ifndef __INTERPOLATOR_FACTORY_H__
#define __INTERPOLATOR_FACTORY_H__

#include <vector>
#include <stdexcept>
#include <armadillo>

#include "Interpolator.h"
#include "RadialBasis.h"

class InterpolatorFactory{

public:

  InterpolatorFactory& set_lambda(const double& lambda) {

    if(lambda < 0) {
      throw std::runtime_error("Error: lambda < 0.");
    }

    this->lambda = lambda;

    return *this;

  }

  InterpolatorFactory& set_control_points(const std::vector<arma::vec>& controlPoints) {

    this->controlPoints = controlPoints;

    return *this;

  }

  InterpolatorFactory& set_target_values(const std::vector<double>& targetValues) {

    this->targetValues = targetValues;

    return *this;

  }

  Interpolator create() const {

    // first verify that control points were provided
    verify();

    arma::vec radialBasisWeights;
    arma::vec polynomialWeights;

    estimate_parameters(radialBasisWeights, polynomialWeights);

    // build Interpolator object
    Interpolator result(this->controlPoints, radialBasisWeights, polynomialWeights);

    return result;

  }

private:

  void estimate_parameters(
                           arma::vec& w,
                           arma::vec& v) const {

    const arma::mat M = compute_system_matrix();
    const arma::vec b = compute_right_handside();

    arma::vec x = arma::solve(M, b);

    // parameters for each sample point
    w = x.subvec(0, this->controlPoints.size() - 1);

    // polynomial part consists of 3 entries
    v = x.subvec(this->controlPoints.size(), this->controlPoints.size() + 2);

  }

  arma::mat compute_system_matrix() const {

    // compute submatrices
    const arma::mat A = compute_A();
    const arma::mat B = compute_B();
    const arma::mat B_T = B.t();

    // compute dimensions
    const int rowAmount = A.n_rows + B_T.n_rows;
    const int colAmount = A.n_cols + B.n_cols;

    // allocate storage
    arma::mat M = arma::zeros(rowAmount, colAmount);

    // add submatrix A
    M.submat(0, 0, A.n_rows - 1, A.n_cols - 1)             = A;
    // shift by column amount of A and add B
    M.submat(0, A.n_cols, B.n_rows - 1, colAmount - 1)     = B;

    // shift by row amount of A and add B_T
    M.submat(A.n_rows, 0, rowAmount - 1, B_T.n_cols - 1) = B_T;

    // the remaining part consists of 0s

    return M;

  }

  arma::mat compute_A() const {

    const int amount = this->controlPoints.size();

    arma::mat A = arma::zeros(amount, amount);

    for(int i = 0; i < amount; ++i) {
      for(int j = 0; j < amount; ++j) {

        const arma::vec& c_i = this->controlPoints.at(i);
        const arma::vec& c_j = this->controlPoints.at(j);

        A(i, j) = RadialBasis::compute(arma::norm(c_i - c_j));

      }
    }

    // compute diagonal matrix with the mean of A as entries
    const arma::mat mean = arma::eye(amount, amount) * arma::accu(A) / pow(amount, 2);

    // modify A to incorporate the regularization term
    A = A + this->lambda * pow(mean, 2);

    return A;

  }
  arma::mat compute_B() const {

    // we have two-dimensional points and a column containing a 1-vector
    const int colAmount = 2 + 1;

    // we have for each sample point a row
    const int rowAmount = this->controlPoints.size();

    arma::mat B = arma::ones(rowAmount, colAmount);

    for(int i = 0; i < rowAmount; ++i) {
      for(int j = 1; j < colAmount; ++j) {

        // get sample point
        const arma::vec& c_i = this->controlPoints.at(i);

        // get corresponding entry of sample point and save in B
        // shift by 1 because we start at index 1 in B
        B(i, j) = c_i(j - 1);

      }
    }

    return B;

  }

  arma::vec compute_right_handside() const {

    // we have entries for the N sample points and 3 for the polynomial
    // part
    arma::vec b = arma::zeros(this->targetValues.size() + 3);

    for(size_t i = 0; i < this->targetValues.size(); ++i ) {

      // get target value
      const double& y_i = this->targetValues.at(i);

      b(i) = y_i;

    }

    // the remaining entries are 0

    return b;

  }

  void verify() const {

    if(this->controlPoints.size() == 0) {
      throw std::runtime_error("Error: No control points provided.");
    }

    if(this->controlPoints.size() != this->targetValues.size()) {
      throw std::runtime_error("Error: Control points and target values size mismatch.");
    }

  }

  double lambda = 0;

  std::vector<arma::vec> controlPoints;

  std::vector<double> targetValues;

};
#endif
