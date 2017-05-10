#ifndef __RIGID_ALIGNMENT_H__
#define __RIGID_ALIGNMENT_H__

#include <stdexcept>
#include <armadillo>

#include "alignment/RigidTransformation.h"
#include "alignment/PointSet.h"
#include "landmark/Landmark.h"

/* class for performing rigid alignment.
 * Based on Timo's code
 */
class RigidAlignment{

  public:

    /*-----------------------------------------------------------------------*/

    RigidAlignment() {
    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment(
        const std::vector<Landmark>& landmarks
        ) {

      for(const auto& elem: landmarks) {
        this->source.push_back(elem.sourcePosition);
        this->target.push_back(elem.targetPosition);
      }

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment(
        const std::vector<arma::vec>& source,
        const std::vector<arma::vec>& target) :
      source(source), target(target) {


      }

    /*-----------------------------------------------------------------------*/

    RigidAlignment(
        const std::vector<double>& source,
        const std::vector<double>& target
        ) :
      source(PointSet::unserialize(source)),
      target(PointSet::unserialize(target)) {


      }

    /*-----------------------------------------------------------------------*/

    RigidAlignment(
        const Mesh& source,
        const Mesh& target
        ) :
      source(source.get_vertices()),
      target(target.get_vertices()) {

      }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_source(const Mesh& source) {

      this->source = source.get_vertices();

      return *this;

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_target(const Mesh& target) {

      this->target = target.get_vertices();

      return *this;

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_source(const std::vector<arma::vec>& source) {

      this->source = source;

      return *this;

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_target(const std::vector<arma::vec>& target) {

      this->target = target;

      return *this;

    }

    /*-----------------------------------------------------------------------*/

    const RigidTransformation& get_transformation() {

      compute_rigid_alignment();
      return this->transformation;

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_rotation(bool value) {

      this->rotation = value;
      return *this;

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_scaling(bool value) {

      this->scaling = value;
      return *this;

    }

    /*-----------------------------------------------------------------------*/


    RigidAlignment& set_translation(bool value) {

      this->translation = value;
      return *this;

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_x_translation(bool value) {

      this->xTranslation = value;
      return *this;

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_y_translation(bool value) {

      this->yTranslation = value;
      return *this;

    }

    /*-----------------------------------------------------------------------*/

    RigidAlignment& set_z_translation(bool value) {

      this->zTranslation = value;
      return *this;

    }

    /*-----------------------------------------------------------------------*/

  private:

    /*-----------------------------------------------------------------------*/

    void compute_rigid_alignment() {

      if( this->source.size() != this->target.size() ) {
        throw std::runtime_error(
            "Source point amount does not match target point amount."
            );
      }

      if( this->source.empty() || this->target.empty() ) {
        throw std::runtime_error(
            "No data provided for alignment."
            );
      }

      this->pointAmount = this->source.size();

      compute_mean(this->source, this->sourceMean);
      compute_mean(this->target, this->targetMean);

      // convert points to local coordinate systems
      points_to_local(this->source, this->sourceMean, this->sourceLocal);
      points_to_local(this->target, this->targetMean, this->targetLocal);

      // set origin of local coordinate system
      this->transformation.set_origin(this->sourceMean);

      if(this->translation) {
        compute_translation();
      }
      if(this->rotation) {
        compute_rotation();
      }
      if(this->scaling) {
        compute_scaling();
      }

    }


    /*-----------------------------------------------------------------------*/

    void compute_translation() {

      arma::vec translation = this->targetMean - this->sourceMean;

      if( this->xTranslation == false) {
        translation(0) = 0;
      }

      if( this->yTranslation == false) {
        translation(1) = 0;
      }

      if( this->zTranslation == false) {
        translation(2) = 0;
      }

      this->transformation.set_translation(translation);

    }

    /*-----------------------------------------------------------------------*/

    void compute_scaling() {

      // if
      // - s is the scaling parameter,
      // - p_i are the source points,
      // - R is a orthonormal rotation matrix, and
      // - q_i = s R p_i are the target points,
      //
      // we have:
      //
      //      ( sum_i || q_i     ||_2 ) / ( sum_i || p_i ||_2 )
      // =  s ( sum_i || R * p_i ||_2 ) / ( sum_i || p_i ||_2 )
      //
      // ( orthonormal matrix does not change length )
      // =  s ( sum_i || p_i ||_2 ) / ( sum_i || p_i ||_2 )
      //
      // = s

      double sourceSum = 0;
      double targetSum = 0;

      for( size_t i = 0; i < this->pointAmount; ++i) {
        sourceSum += arma::norm(this->sourceLocal.at(i));
        targetSum += arma::norm(this->targetLocal.at(i));
      }

      const double scale = ( sourceSum != 0)? targetSum / sourceSum : 1.;

      this->transformation.set_scaling(scale);

    }

    /*-----------------------------------------------------------------------*/

    void compute_rotation() {

      arma::mat A(this->pointAmount, 3);
      arma::mat B(this->pointAmount, 3);

      for(size_t i = 0; i < this->pointAmount; ++i) {
        // A contains the source points as rows
        A.row(i) = arma::trans(this->sourceLocal.at(i));
        // B containes the target points as rows
        B.row(i) = arma::trans(this->targetLocal.at(i));
      }

      // solve system A * X = B
      auto X = arma::solve(A, B);

      // now, get rid of the scaling
      arma::mat U, V;
      arma::vec s;

      // perform singular value decomposition to obtain
      //
      // X = U S V^T
      //
      // S is a diagonal scaling matrix
      arma::svd( U, s, V, X );

      // reconstruct rotation matrix without scaling component and
      // transpose it
      arma::mat rotationMatrix = ( U * V.t() ).t();

      if( arma::det(rotationMatrix) <= -1) {
        throw std::runtime_error("Determinant <= -1");
      }

      this->transformation.set_rotation(rotationMatrix);

    }

    /*-----------------------------------------------------------------------*/

    void points_to_local(
        const std::vector<arma::vec>& points,
        const arma::vec& origin,
        std::vector<arma::vec>& pointsLocal
        ) {

      // empty data container
      pointsLocal.clear();

      for( const auto& point: points) {
        pointsLocal.push_back(point - origin);
      }

    }

    /*-----------------------------------------------------------------------*/

    void compute_mean(
        const std::vector<arma::vec>& points,
        arma::vec& mean
        ) {

      mean = arma::zeros<arma::vec>(3);

      for( const auto& point: points) {
        mean += point;
      }

      mean /= points.size();

    }

    /*-----------------------------------------------------------------------*/

    bool rotation = true;
    bool scaling = true;
    bool translation = true;

    bool xTranslation = true;
    bool yTranslation = true;
    bool zTranslation = true;

    size_t pointAmount;

    std::vector<arma::vec> source;
    std::vector<arma::vec> target;

    arma::vec sourceMean;
    arma::vec targetMean;

    std::vector<arma::vec> sourceLocal;
    std::vector<arma::vec> targetLocal;

    RigidTransformation transformation;
};
#endif
