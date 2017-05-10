#ifndef __PROFILE_MESH_H__
#define __PROFILE_MESH_H__

#include <armadillo>

#include "mesh/Mesh.h"
#include "scan/Scan.h"
#include "mesh/NormalEstimation.h"

/* class that decorates a mesh with color profiles at each vertex using the
 * vertex normal at a scan with color information
 */
class ProfileMesh : public Mesh {

  public:

    /*-----------------------------------------------------------------------*/

    ProfileMesh(const Mesh& mesh, const Scan& scan) : Mesh(mesh), scan(scan) {

      this->offset = 0;
      this->length = 5;

      // compute normals if they are not present
      if( this->vertexNormals.size() == 0 ) {
        NormalEstimation estimator(*this);
        this->vertexNormals = estimator.compute();
      }

      // get origin of scan
      const double orgX = scan.data()->get_org_x() * scan.data()->get_hx();
      const double orgY = scan.data()->get_org_y() * scan.data()->get_hy();
      const double orgZ = scan.data()->get_org_z() * scan.data()->get_hz();

      this->origin = arma::vec{orgX, orgY, orgZ};
       
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
        result(i) =
          // make sure to map the coordinate to the local coordinate system
          // of the scan
          this->scan.access()->get_value_coordinate_neumann(
            vertex + (i + offset) * normal - this->origin);
      }

      return result;
    }

    /*-----------------------------------------------------------------------*/

    std::vector<arma::vec> profiles;
    const Scan& scan;

    arma::vec origin;

    int length;
    double offset;

    /*-----------------------------------------------------------------------*/

};
#endif
