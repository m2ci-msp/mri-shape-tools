#ifndef __CLOUD_BOUNCER_H__
#define __CLOUD_BOUNCER_H__

#include "mesh/Mesh.h"
#include "alignment/KdTree.h"

class CloudBouncer {

public:

  /*--------------------------------------------------------------------------*/

  CloudBouncer(const Mesh& mesh) : kdTree(KdTree(mesh.get_vertices())) {
    this->boundary = mesh.get_vertices();
    this->boundaryNormals = mesh.get_vertex_normals();

    compute_side_boundary();

  }

  /*--------------------------------------------------------------------------*/

  CloudBouncer& set_bounce_sides(const bool& bounceSides) {

    this->bounceSides = bounceSides;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  CloudBouncer& set_side_distance(const double& distance) {

    this->sideDistance = distance;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  CloudBouncer& set_maximum_distance(const double& distance) {

    this->maxDistance = distance;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  CloudBouncer& set_keep_points_below(const bool& keep) {

    this->keepPointsBelow = keep;

    return *this;

  }

  /*--------------------------------------------------------------------------*/

  Mesh bounce(const Mesh& mesh) {

    const std::vector<arma::vec> oldVertices = mesh.get_vertices();
    const std::vector<arma::vec> oldNormals = mesh.get_vertex_normals();

    std::vector<arma::vec> newVertices;
    std::vector<arma::vec> newNormals;

    for( size_t i = 0; i < oldVertices.size(); ++i) {
      if( can_stay(oldVertices.at(i)) == true ) {
        newVertices.push_back(oldVertices.at(i));
        newNormals.push_back(oldNormals.at(i));
      }
    }

    Mesh result;
    result.set_vertices(newVertices)\
      .set_vertex_normals(newNormals);

    return result;

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  bool can_stay(const arma::vec& point) const {

    const int index = this->kdTree.get_nearest_neighbor_index(point);

    const arma::vec edge = (point - boundary.at(index));

    if( this->bounceSides == true ) {

      if( point(2) < this->left  - this->sideDistance ||
          point(2) > this->right + this->sideDistance ) {

        return false;

      }
    }
    // keep points below the boundary coordinate if desired
    if( this->keepPointsBelow == true && edge(1) > 0 ) {
      return true;
    }

    // remove points with a distance above the maximally allowed one from the plane
    if( arma::dot(edge, this->boundaryNormals.at(index) ) > this->maxDistance )  {
      return false;
    }


    return true;

  }

  /*--------------------------------------------------------------------------*/

  void compute_side_boundary() {

    for(const arma::vec vertex: this->boundary) {

      this->left  = ( vertex(2) < this->left  )? vertex(2) : this->left ;
      this->right = ( vertex(2) > this->right )? vertex(2) : this->right;
    }

  }

  /*--------------------------------------------------------------------------*/

  bool bounceSides = false;

  std::vector<arma::vec> boundary;
  std::vector<arma::vec> boundaryNormals;
  KdTree kdTree;

  double sideDistance = 1;
  double maxDistance = 1;
  bool keepPointsBelow = false;

  double left = DBL_MAX;
  double right = -DBL_MAX;

  /*--------------------------------------------------------------------------*/

};

#endif
