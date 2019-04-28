#ifndef __RIGID_ALIGNMENT_TRANSFORMATION_H__
#define __RIGID_ALIGNMENT_TRANSFORMATION_H__

#include <armadillo>

#include "matrix/Rotation.h"
#include "matrix/Scaling.h"
#include "matrix/Translation.h"


namespace rigidAlignment{

  class Transformation{

  public:

    /*--------------------------------------------------------------------------*/

    Transformation(
      const Translation& translation,
      const Scaling& scaling,
      const double& alpha,
      const double& beta,
      const double& gamma
      ) :
      translation(translation),
      scaling(scaling),
      rotationX(1, 0, 0, alpha),
      rotationY(0, 1, 0, beta),
      rotationZ(0, 0, 1, gamma) {

    }

    /*--------------------------------------------------------------------------*/

    Transformation() {
    }

    /*--------------------------------------------------------------------------*/

    void set_origin(const arma::vec& origin) {

      this->toLocal = Translation(-origin(0), -origin(1), -origin(2));
      this->toGlobal = Translation(origin(0), origin(1), origin(2));

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_matrix(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *

        this->toGlobal.get_matrix() *

        this->scaling.get_matrix() *

        this->rotationX.get_matrix() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_alpha(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *

        this->toGlobal.get_matrix() *

        this->scaling.get_matrix() *

        this->rotationX.get_derivative_theta() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_beta(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *

        this->toGlobal.get_matrix() *

        this->scaling.get_matrix() *

        this->rotationX.get_matrix() *
        this->rotationY.get_derivative_theta() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_gamma(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *

        this->toGlobal.get_matrix() *

        this->scaling.get_matrix() *

        this->rotationX.get_matrix() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_derivative_theta() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/


    arma::vec apply_derivative_sx(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *

        this->toGlobal.get_matrix() *

        this->scaling.get_derivative_sx() *

        this->rotationX.get_matrix() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }
    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_sy(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *

        this->toGlobal.get_matrix() *

        this->scaling.get_derivative_sy() *

        this->rotationX.get_matrix() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_sz(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *

        this->toGlobal.get_matrix() *

        this->scaling.get_derivative_sz() *

        this->rotationX.get_matrix() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_tx(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_derivative_tx() *

        this->toGlobal.get_matrix() *

        this->scaling.get_matrix() *

        this->rotationX.get_matrix() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_ty(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_derivative_ty() *

        this->toGlobal.get_matrix() *

        this->scaling.get_matrix() *

        this->rotationX.get_matrix() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);


    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_tz(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_derivative_tz() *

        this->toGlobal.get_matrix() *

        this->scaling.get_matrix() *

        this->rotationX.get_matrix() *
        this->rotationY.get_matrix() *
        this->rotationZ.get_matrix() *

        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    arma::vec convert_to_homogeneous(const arma::vec& p) const {

      return arma::vec{p(0), p(1), p(2), 1};

    }

    /*--------------------------------------------------------------------------*/

    arma::vec convert_to_euclidean(const arma::vec& p) const {

      return arma::vec{p(0), p(1), p(2)};

    }

    /*--------------------------------------------------------------------------*/

    Translation toLocal;
    Translation toGlobal;

    Translation translation;

    Scaling scaling;

    Rotation rotationX;
    Rotation rotationY;
    Rotation rotationZ;


    /*--------------------------------------------------------------------------*/

  };

}
#endif
