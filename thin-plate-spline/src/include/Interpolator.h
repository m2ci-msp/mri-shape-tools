#ifndef __INTERPOLATOR_H__
#define __INTERPOLATOR_H__

#include <vector>
#include <armadillo>

#include "RadialBasis.h"

class Interpolator{

public:
  Interpolator(
               const std::vector<arma::vec>& controlPoints,
               const arma::vec& radialBasisWeights,
               const arma::vec& polynomialWeights
               ) :
    controlPoints(controlPoints),
    radialBasisWeights(radialBasisWeights),
    polynomialWeights(polynomialWeights) {
    }

  double estimate(const double& x, const double& y) const {

    const arma::vec position({x, y,});

    const double result = radial_basis_part(position) + polynomial_part(position);

    return result;

  }

private:

  double radial_basis_part(const arma::vec& x) const {

    double projection = 0;

    for(size_t i = 0; i < this->controlPoints.size(); ++i) {

      const double& w = this->radialBasisWeights(i);
      const arma::vec& c = this->controlPoints.at(i);

      projection += w * RadialBasis::compute(arma::norm(x - c));

    }

    return projection;

  }

  double polynomial_part(const arma::vec& x) const {

    // x with added 1 entry at the beginning
    const arma::vec x1({1, x(0), x(1)});

    return arma::dot(this->polynomialWeights, x1);

  }

  const std::vector<arma::vec> controlPoints;
  const arma::vec radialBasisWeights;
  const arma::vec polynomialWeights;

};
#endif
