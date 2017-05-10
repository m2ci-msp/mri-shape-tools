#ifndef __CROSS_PRODUCT_MATRIX_PART_H__
#define __CROSS_PRODUCT_MATRIX_PART_H__

#include <armadillo>

class CrossProductMatrixPart{

public:

  /*--------------------------------------------------------------------------*/

  CrossProductMatrixPart() {
  }

  /*--------------------------------------------------------------------------*/

  CrossProductMatrixPart(
    const double& ax,
    const double& ay,
    const double& az
    ) {

    this->A = arma::mat{

      {    0, - az,   ay },
      {   az,    0, - ax },
      { - ay,   ax,    0 }

    };

    this->A_ax = arma::mat{

      { 0, 0,   0 },
      { 0, 0, - 1 },
      { 0, 1,   0 }

    };

    this->A_ay = arma::mat{

      {   0, 0, 1 },
      {   0, 0, 0 },
      { - 1, 0, 0 }

    };

    this->A_az = arma::mat{

      { 0, - 1, 0 },
      { 1,   0, 0 },
      { 0,   0, 0 }

    };

  }

  /*--------------------------------------------------------------------------*/

  const arma::mat& get_matrix() const { return A; }
  const arma::mat& get_derivative_ax() const { return A_ax; }
  const arma::mat& get_derivative_ay() const { return A_ay; }
  const arma::mat& get_derivative_az() const { return A_az; }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  arma::mat A;
  arma::mat A_ax;
  arma::mat A_ay;
  arma::mat A_az;

  /*--------------------------------------------------------------------------*/


};

#endif
