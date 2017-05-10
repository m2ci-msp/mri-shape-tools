#ifndef __RIGID_TRANSFORMATION_H__
#define __RIGID_TRANSFORMATION_H__

#include <armadillo>
#include "mesh/Mesh.h"
#include "landmark/Landmark.h"

/* class representing a rigid transformation */
class RigidTransformation{

  public:

    RigidTransformation() {

      this->scaling = 1.;
      this->translation = arma::zeros<arma::vec>(3);
      this->origin = arma::zeros<arma::vec>(3);
      this->rotation = arma::eye<arma::mat>(3, 3);

    }

    /*-----------------------------------------------------------------------*/

    RigidTransformation& set_scaling(const double& scaling) {
      this->scaling = scaling;
      return *this;
    }

    /*-----------------------------------------------------------------------*/

    RigidTransformation& set_translation(const arma::vec& translation) {
      this->translation = translation;
      return *this;
    }

    /*-----------------------------------------------------------------------*/

    RigidTransformation& set_translation(
      const double& x, const double& y, const double& z) {

      this->translation = arma::vec({x, y, z});
      return *this;
    }

    /*-----------------------------------------------------------------------*/


    RigidTransformation& set_origin(const arma::vec& origin) {
      this->origin = origin;
      return *this;
    }

    /*-----------------------------------------------------------------------*/

    RigidTransformation& set_rotation(const arma::mat& rotation) {
      this->rotation = rotation;
      return *this;
    }

    /*-----------------------------------------------------------------------*/

    RigidTransformation& set_rotation(
      const double& alpha, const double& beta, const double& gamma) {

      arma::mat rotationX = create_x_rotation(alpha);
      arma::mat rotationY = create_y_rotation(beta);
      arma::mat rotationZ = create_z_rotation(gamma);

      this->rotation = rotationX * rotationY * rotationZ;
      return *this;
    }


   /*-----------------------------------------------------------------------*/



    RigidTransformation& set_z_rotation(const double& degrees) {
      const double angle = degrees / 180. * M_PI;
      const double cosAngle = cos(angle);
      const double sinAngle = sin(angle);

      this->rotation
        << cosAngle << -sinAngle << 0 << arma::endr
        << sinAngle << cosAngle << 0 << arma::endr
        << 0 << 0 << 1 << arma::endr;

      return *this;

    }

    /*-----------------------------------------------------------------------*/

    double get_scaling() const {
      return this->scaling;
    }

    /*-----------------------------------------------------------------------*/

    const arma::vec& get_translation() const {
      return this->translation;
    }

    /*-----------------------------------------------------------------------*/

    const arma::vec& get_origin() const {
      return this->origin;
    }

    /*-----------------------------------------------------------------------*/

    const arma::mat& get_rotation() const {
      return this->rotation;
    }

    /*-----------------------------------------------------------------------*/

    void apply(arma::vec& point) const {
        point = transform_point(point);
    }

    /*-----------------------------------------------------------------------*/

    void apply(std::vector<arma::vec>& points) const {

      for(auto& elem : points) {
        elem = transform_point(elem);
      }

    }

    /*-----------------------------------------------------------------------*/

    void apply(std::vector<Landmark>& landmarks) const {

        for(auto& elem: landmarks) {
        elem.targetPosition = transform_point(elem.targetPosition);
        }

    }

    /*-----------------------------------------------------------------------*/

    void apply(Mesh& mesh) const {

      std::vector<arma::vec>& points = mesh.get_vertices();
      std::vector<arma::vec>& normals = mesh.get_vertex_normals();

      for(auto& elem : points) {
        elem = transform_point(elem);
      }

      for(auto& elem: normals) {
        elem = transform_normal(elem);
      }

    }

    /*-----------------------------------------------------------------------*/

    void apply_linearized(arma::vec& linearizedPoints) const {

      const int pointAmount = linearizedPoints.size() / 3;

      for(int i = 0; i < pointAmount; ++i) {

        double& x = linearizedPoints(i * 3    );
        double& y = linearizedPoints(i * 3 + 1);
        double& z = linearizedPoints(i * 3 + 2);

        arma::vec point({x, y, z});

        point = transform_point(point);

        x = point(0);
        y = point(1);
        z = point(2);

      }

    }

    /*-----------------------------------------------------------------------*/

    std::vector<arma::vec> get_transformed(const std::vector<arma::vec>& original) const {

        std::vector<arma::vec> result;

        for(const arma::vec& point: original) {

        result.push_back(transform_point(point));

        }

        return result;
    }

    /*-----------------------------------------------------------------------*/

  // FIXME: seems to be broken!
    void invert() {

      this->translation = -this->translation;
      this->scaling = 1. / this->scaling;
      this->rotation = this->rotation.t();

    }

    /*-----------------------------------------------------------------------*/

    bool equal_to(
        const RigidTransformation& other,
        const double delta = 0
        ) const {

      arma::vec originDifference =
        arma::abs(this->origin - other.origin);

      arma::vec translationDifference =
        arma::abs(this->translation - other.translation);

      const double scalingDifference = fabs(this->scaling - other.scaling);

      arma::vec rotationDifference =
        arma::vectorise(
            arma::abs(this->rotation - other.rotation)
            );

      if(
          arma::any(translationDifference > delta ) ||
          arma::any(originDifference      > delta ) ||
          arma::any(rotationDifference    > delta ) ||
                    scalingDifference     > delta
        ) {
        return false;
      }

      return true;

    }

    /*-----------------------------------------------------------------------*/

    void combine_with(const RigidTransformation& other) {

      this->scaling *= other.get_scaling();
      this->rotation *= other.get_rotation();
      this->translation += other.get_translation();

    }

    /*-----------------------------------------------------------------------*/

  private:

    /*-----------------------------------------------------------------------*/

    arma::mat create_x_rotation(const double& degrees) const {

      const double angle = degrees / 180. * M_PI;
      const double cosAngle = cos(angle);
      const double sinAngle = sin(angle);

      arma::mat matrix;

      matrix
      << 1 << 0        << 0         << arma::endr
      << 0 << cosAngle << -sinAngle << arma::endr
      << 0 << sinAngle << cosAngle  << arma::endr;

      return matrix;
    }

    /*-----------------------------------------------------------------------*/

    arma::mat create_y_rotation(const double& degrees) const {

      const double angle = degrees / 180. * M_PI;
      const double cosAngle = cos(angle);
      const double sinAngle = sin(angle);

      arma::mat matrix;

      matrix
      << cosAngle  << 0 << sinAngle  << arma::endr
      << 0         << 1 << 0         << arma::endr
      << -sinAngle << 0 << cosAngle  << arma::endr;

      return matrix;
    }

    /*-----------------------------------------------------------------------*/

    arma::mat create_z_rotation(const double& degrees) const {

      const double angle = degrees / 180. * M_PI;
      const double cosAngle = cos(angle);
      const double sinAngle = sin(angle);

      arma::mat matrix;

      matrix
      << cosAngle  << -sinAngle << 0 << arma::endr
      << sinAngle  << cosAngle  << 0 << arma::endr
      << 0         << 0         << 1 << arma::endr;

      return matrix;
    }


    /*-----------------------------------------------------------------------*/

    arma::vec transform_point(const arma::vec& point) const {

      // convert point to local coordinate system
      const arma::vec pointLocal = point - this->origin;

      // rotate and scale point
      const arma::vec pointRotatedAndScaled =
        this->scaling * this->rotation * pointLocal;

      // move back to global coordinate system
      arma::vec result = this->origin + pointRotatedAndScaled;

      // translate point
      result += this->translation;

      return result;

    }

    /*-----------------------------------------------------------------------*/

    arma::vec transform_normal(const arma::vec& normal) const {
      return this->rotation * normal;
    }

    /*-----------------------------------------------------------------------*/


    double scaling;
    arma::vec translation;

    // origin of local coordinate system
    arma::vec origin;
    arma::mat rotation;
};
#endif
