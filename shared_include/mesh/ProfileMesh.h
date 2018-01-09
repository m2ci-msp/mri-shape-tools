#ifndef __PROFILE_MESH_H__
#define __PROFILE_MESH_H__

#include <armadillo>

#include "mesh/Mesh.h"
#include "image/Image.h"
#include "mesh/NormalEstimation.h"

/* class that decorates a mesh with color profiles at each vertex using the
 * vertex normal at a scan with color information
 */
class ProfileMesh : public Mesh {

public:

  /*-----------------------------------------------------------------------*/

  ProfileMesh(const Mesh& mesh, Image& scan) : Mesh(mesh), scan(scan) {

    this->offset = 0;
    this->length = 5;

    // compute normals if they are not present
    if( this->vertexNormals.size() == 0 ) {
      NormalEstimation estimator(*this);
      this->vertexNormals = estimator.compute();
    }

  }

  /*-----------------------------------------------------------------------*/

  ProfileMesh& set_offset(double offset) {

    this->offset = offset;
    return *this;

  }

  /*-----------------------------------------------------------------------*/

  ProfileMesh& set_length(int length) {

    this->length = length;
    return *this;

  }

  /*-----------------------------------------------------------------------*/

  void compute_profiles() {

    // change boundary of scan and mirror
    const double size = this->offset + this->length;
    this->scan.boundary().change(size, size, size);

    this->scan.mirror().all();

    for(size_t id = 0; id < this->vertices.size(); ++id) {

      const arma::vec& vertex = this->vertices.at(id);
      const arma::vec& normal = this->vertexNormals.at(id);

      const arma::vec profile = compute_profile(vertex, normal);

      this->profiles.push_back(profile);

    }
  }
    
  /*-----------------------------------------------------------------------*/

  std::vector<arma::vec> get_profiles() const {

    return this->profiles;

  }

  /*-----------------------------------------------------------------------*/

private:

  /*-----------------------------------------------------------------------*/

  arma::vec compute_profile(
                            const arma::vec& vertex,
                            const arma::vec& normal) const {

    arma::vec result = arma::zeros<arma::vec>(this->length);

    for(int i = 0; i < this->length; ++i) {

      const arma::vec position = vertex + (i + offset) * normal;

      result(i) =

        this->scan.access().at_coordinate( position(0), position(1), position(2) );

    }

    return result;
  }

  /*-----------------------------------------------------------------------*/

  std::vector<arma::vec> profiles;
  Image& scan;

  int length;
  double offset;

  /*-----------------------------------------------------------------------*/

};
#endif
