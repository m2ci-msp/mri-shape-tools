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
      const Rotation& rotation,
      const Scaling& scaling,
      const Translation& translation
      ) :
      rotation(rotation), scaling(scaling), translation(translation) {

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
        //  this->scaling.get_matrix() *
        this->rotation.get_matrix() *
        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_theta(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *
        this->toGlobal.get_matrix() *
//        this->scaling.get_matrix() *
        this->rotation.get_derivative_theta() *
        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_ax(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *
        this->toGlobal.get_matrix() *
        this->rotation.get_derivative_ax() *
        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_ay(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *
        this->toGlobal.get_matrix() *
        this->rotation.get_derivative_ay() *
        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_az(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *
        this->toGlobal.get_matrix() *
//        this->scaling.get_matrix() *
        this->rotation.get_derivative_az() *
        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_s(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_matrix() *
        this->toGlobal.get_matrix() *
//        this->scaling.get_derivative_s() *
        this->rotation.get_matrix() *
        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_tx(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_derivative_tx() *
        this->toGlobal.get_matrix() *
//        this->scaling.get_matrix() *
        this->rotation.get_matrix() *
        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);

    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_ty(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_derivative_ty() *
        this->toGlobal.get_matrix() *
//        this->scaling.get_matrix() *
        this->rotation.get_matrix() *
        this->toLocal.get_matrix();

      const arma::vec ph = convert_to_homogeneous(p);

      return convert_to_euclidean(A * ph);


    }

    /*--------------------------------------------------------------------------*/

    arma::vec apply_derivative_tz(const arma::vec& p) const {

      const arma::mat A =
        this->translation.get_derivative_tz() *
        this->toGlobal.get_matrix() *
//        this->scaling.get_matrix() *
        this->rotation.get_matrix() *
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

    Rotation rotation;
    Scaling scaling;
    Translation translation;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
