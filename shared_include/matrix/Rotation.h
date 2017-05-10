#ifndef __ROTATION_H__
#define __ROTATION_H__

#include <cmath>
#include <armadillo>

#include "matrix/CrossProductPart.h"
#include "matrix/CrossProductMatrixPart.h"

/* class for rotation matrices built from axis-angle representations
 *
 */

class Rotation{

public:

  /*--------------------------------------------------------------------------*/

  Rotation(
    const double& ax, const double& ay, const double& az, const double& theta) :
    ax(ax), ay(ay), az(az), theta(theta), cosTheta(cos(theta)), sinTheta(sin(theta)) {

    init();

  }

  /*--------------------------------------------------------------------------*/

  Rotation() :
    ax(1), ay(0), az(0), theta(0), cosTheta(cos(0)), sinTheta(sin(0)) {

    init();

  }

  /*--------------------------------------------------------------------------*/

  arma::mat get_matrix() const {

    const double& c = this->cosTheta;
    const double& s = this->sinTheta;

    arma::mat result = arma::eye(4, 4);

    result.submat(0, 0, 2, 2) =

              c * arma::eye(3, 3) +
              s * this->scale        * this->crossMatrix.get_matrix() +
      ( 1 - c ) * this->scaleSquared * this->cross.get_matrix();

    return result;

  }

  /*--------------------------------------------------------------------------*/

  // derivative w.r.t. the angle theta
  arma::mat get_derivative_theta() const {

    const double c = - this->sinTheta;
    const double s = this->cosTheta;

    arma::mat result = arma::zeros(4, 4);

    result.submat(0, 0, 2, 2) =

              c * arma::eye(3, 3) +
              s * this->scale        * this->crossMatrix.get_matrix() +
      ( 0 - c ) * this->scaleSquared * this->cross.get_matrix();

    return result;

  }

  /*--------------------------------------------------------------------------*/

  // derivative w.r.t. the axis component ax
  arma::mat get_derivative_ax() const {

    const double& c = cosTheta;
    const double& s = sinTheta;

    arma::mat result = arma::zeros(4, 4);

    result.submat(0, 0, 2, 2) =

      // product rule: f' g
      s * 2 * ax * this->scaleOuterDerivative * this->crossMatrix.get_matrix() +
      // + f g'
      s * this->scale * this->crossMatrix.get_derivative_ax() +
      // product rule: f' g
      ( 1 - c ) * 2 * ax * this->scaleSquaredOuterDerivative * this->cross.get_matrix() +
      // f g'
      ( 1 - c ) * this->scaleSquared * this->cross.get_derivative_ax();

    return result;

 }

  /*--------------------------------------------------------------------------*/

  // derivative w.r.t. the axis component ay
  arma::mat get_derivative_ay() const {

    const double& c = cosTheta;
    const double& s = sinTheta;

    arma::mat result = arma::zeros(4, 4);

    result.submat(0, 0, 2, 2) =

      // product rule: f' g
      s * 2 * ay * this->scaleOuterDerivative * this->crossMatrix.get_matrix() +
      // + f g'
      s * this->scale * this->crossMatrix.get_derivative_ay() +
      // product rule: f' g
      ( 1 - c ) * 2 * ay * this->scaleSquaredOuterDerivative * this->cross.get_matrix() +
      // f g'
      ( 1 - c ) * this->scaleSquared * this->cross.get_derivative_ay();

    return result;

  }

  /*--------------------------------------------------------------------------*/

  // derivative w.r.t. the axis component az
  arma::mat get_derivative_az() const {

    const double& c = cosTheta;
    const double& s = sinTheta;

    arma::mat result = arma::zeros(4, 4);

    result.submat(0, 0, 2, 2) =

      // product rule: f' g
      s * 2 * az * this->scaleOuterDerivative * this->crossMatrix.get_matrix() +
      // + f g'
      s * this->scale * this->crossMatrix.get_derivative_az() +
      // product rule: f' g
      ( 1 - c ) * 2 * az * this->scaleSquaredOuterDerivative * this->cross.get_matrix() +
      // f g'
      ( 1 - c ) * this->scaleSquared * this->cross.get_derivative_az();

    return result;

  }

  /*--------------------------------------------------------------------------*/


protected:

  /*--------------------------------------------------------------------------*/

  void init() {

    this->cross = CrossProductPart(ax, ay, az);
    this->crossMatrix = CrossProductMatrixPart(ax, ay, az);

    const double squaredNorm = pow(ax, 2) + pow(ay, 2) + pow(az, 2);

    this->scale = 1. / sqrt(squaredNorm);
    this->scaleSquared = 1. / squaredNorm;

    this->scaleOuterDerivative =
      - 0.5 * pow( squaredNorm, -1.5 );

    this->scaleSquaredOuterDerivative =
      - 1 * pow( squaredNorm, -2 );

  }

  /*--------------------------------------------------------------------------*/

  // Rotation axis
  double ax;
  double ay;
  double az;

  // Rotation angle
  double theta;

  // cosine and sine
  double cosTheta;
  double sinTheta;

  CrossProductPart cross;
  CrossProductMatrixPart crossMatrix;
  double scale;
  double scaleSquared;
  double scaleOuterDerivative;
  double scaleSquaredOuterDerivative;

  /*--------------------------------------------------------------------------*/

};

#endif
