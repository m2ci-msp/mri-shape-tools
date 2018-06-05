#ifndef __MATCH_TEMPLATE_TRANSFORMATION_H__
#define __MATCH_TEMPLATE_TRANSFORMATION_H__

#include <armadillo>

#include "matrix/Rotation.h"
#include "matrix/Translation.h"

namespace matchTemplate{

  class Transformation{

  public:

    /*--------------------------------------------------------------------------*/

    Transformation(
      const Rotation& rotation,
      const Translation& translation
      ) :
      rotation(rotation), translation(translation) {

    }

    /*--------------------------------------------------------------------------*/

    Transformation() {
    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_matrix(const arma::vec& p) const {

      Translation toLocal(-p(0), -p(1), -p(2));
      Translation toGlobal(p(0), p(1), p(2));

      const arma::mat A =
        this->translation.get_matrix() *
        toGlobal.get_matrix() *
        this->rotation.get_matrix() *
        toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_theta(const arma::vec& p) const {

      Translation toLocal(-p(0), -p(1), -p(2));
      Translation toGlobal(p(0), p(1), p(2));

      const arma::mat A =
        this->translation.get_matrix() *
        toGlobal.get_matrix() *
        this->rotation.get_derivative_theta() *
        toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_ax(const arma::vec& p) const {

      Translation toLocal(-p(0), -p(1), -p(2));
      Translation toGlobal(p(0), p(1), p(2));

      const arma::mat A =
        this->translation.get_matrix() *
        toGlobal.get_matrix() *
        this->rotation.get_derivative_ax() *
        toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_ay(const arma::vec& p) const {

      Translation toLocal(-p(0), -p(1), -p(2));
      Translation toGlobal(p(0), p(1), p(2));

      const arma::mat A =
        this->translation.get_matrix() *
        toGlobal.get_matrix() *
        this->rotation.get_derivative_ay() *
        toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_az(const arma::vec& p) const {

      Translation toLocal(-p(0), -p(1), -p(2));
      Translation toGlobal(p(0), p(1), p(2));

      const arma::mat A =
        this->translation.get_matrix() *
        toGlobal.get_matrix() *
        this->rotation.get_derivative_az() *
        toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_tx(const arma::vec& p) const {

      Translation toLocal(-p(0), -p(1), -p(2));
      Translation toGlobal(p(0), p(1), p(2));

      const arma::mat A =
        this->translation.get_derivative_tx() *
        toGlobal.get_matrix() *
        this->rotation.get_matrix() *
        toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_ty(const arma::vec& p) const {

      Translation toLocal(-p(0), -p(1), -p(2));
      Translation toGlobal(p(0), p(1), p(2));

      const arma::mat A =
        this->translation.get_derivative_ty() *
        toGlobal.get_matrix() *
        this->rotation.get_matrix() *
        toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);


    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_tz(const arma::vec& p) const {

      Translation toLocal(-p(0), -p(1), -p(2));
      Translation toGlobal(p(0), p(1), p(2));

      const arma::mat A =
        this->translation.get_derivative_tz() *
        toGlobal.get_matrix() *
        this->rotation.get_matrix() *
        toLocal.get_matrix();

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

    Rotation rotation;
    Translation translation;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
