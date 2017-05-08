#ifndef __CREATE_POINT_CLOUD_H__
#define __CREATE_POINT_CLOUD_H__

#include "scan/Scan.h"
#include "mesh/Mesh.h"

class CreatePointCloud{

  public:

    /*----------------------------------------------------------------------*/

    CreatePointCloud(const Scan& scan) :

      scan(scan) {

        this->flip = false;
        this->radius = 2;

      }

    /*----------------------------------------------------------------------*/

    CreatePointCloud& set_flip(const bool& flip) {

      this->flip = flip;

      return *this;
    }

    /*----------------------------------------------------------------------*/

    CreatePointCloud& set_radius(const double& radius) {

      this->radius = radius;

      return *this;
    }

    /*----------------------------------------------------------------------*/

    Mesh create_point_cloud() {

      Mesh pointCloud;

      std::vector<arma::vec> vertices;
      std::vector<arma::vec> normals;

      const int nx = this->scan.data()->get_nx();
      const int ny = this->scan.data()->get_ny();
      const int nz = this->scan.data()->get_nz();

      const double hx = this->scan.data()->get_hx();
      const double hy = this->scan.data()->get_hy();
      const double hz = this->scan.data()->get_hz();

      const arma::vec origin({
          this->scan.data()->get_org_x() * hx,
          this->scan.data()->get_org_y() * hy,
          this->scan.data()->get_org_z() * hz
          });

      // iterate over all voxels
      for(int i = 0; i < nx; ++i) {
        for(int j = 0; j < ny; ++j) {
          for(int k = 0; k < nz; ++k) {

            // check if the current voxel is a surface point
            if( is_surface_point(i, j, k) ) {

              // compute coordinate
              arma::vec point({i * hx, j * hy, k * hz});
              vertices.push_back(origin + point);

              // derive normal
              arma::vec normal = derive_normal(i, j, k);
              normals.push_back(normal);

            }

          } // end for k
        } // end for j
      } // end for i

      pointCloud.set_vertices(vertices);
      pointCloud.set_vertex_normals(normals);

      return pointCloud;

    }

    /*----------------------------------------------------------------------*/

  private:

    /*----------------------------------------------------------------------*/

    /* heuristic for surface point:
     * point itself belongs to surface class ( value 255 )
     * one of its 26 neighbors does not belong to the surface class
     */
    bool is_surface_point(
        const int& x,
        const int& y,
        const int& z
        ) const {

      if( this->scan.access()->get_value_index(x, y, z) != 255 ) {
        return false;
      }

      return (
        this->scan.access()->get_value_index_neumann( x - 1, y, z) != 255 ||
        this->scan.access()->get_value_index_neumann( x + 1, y, z) != 255 ||
        this->scan.access()->get_value_index_neumann( x    , y - 1, z) != 255 ||
        this->scan.access()->get_value_index_neumann( x    , y + 1, z) != 255 ||
        this->scan.access()->get_value_index_neumann( x    , y, z - 1) != 255 ||
        this->scan.access()->get_value_index_neumann( x    , y, z + 1) != 255
        );


      /*
      for( int i = -1; i <= 1; ++i) {
        for( int j = -1; j <= 1; ++j) {
          for( int k = -1; k <= 1; ++k) {
            if( this->scan.access()->get_value_index_neumann(
                  x + i, y + j, z + k) != 255  ) {
              return true;
            }
          }
        }
      }
      */

      // return false;

    }

    /*----------------------------------------------------------------------*/

    arma::vec derive_normal(
        const int& x,
        const int& y,
        const int& z
        ) const {

      // estimate normal direction
      arma::vec normal = estimate_principal_direction(x, y, z);

      // let the normals point outside by default
      if( points_inside(normal, x, y, z)) {
        normal *= -1;
      }

      // flip if wanted by user
      if( this->flip ) {
        normal *= -1;
      }

      return normal;

    }

    /*----------------------------------------------------------------------*/

    arma::vec estimate_principal_direction(
        const int& x,
        const int& y,
        const int& z
        ) const {

      // output gradient at current voxel if radius is 0
      if( radius == 0 ) {
        arma::vec central = this->scan.analysis()->compute_gradient(x, y, z);
        return arma::normalise(central);
      }

      // derive subspace of local principal gradient directions
      const arma::mat structureTensor =
        this->scan.analysis()->compute_region_structure_tensor(
          x, y, z, this->radius
          );

      // perform eigenvalue decomposition to extract most dominant direction
      arma::mat eigv;
      arma::vec eigval;

      arma::eig_sym(eigval, eigv, structureTensor);

      const arma::vec direction = eigv.col(2);

      return direction;

    }

    /*----------------------------------------------------------------------*/

    /* checks if the normal is pointing to the inside */
    bool points_inside(
        const arma::vec& direction,
        const int& x,
        const int& y,
        const int& z
        ) const {

      const double target = this->scan.access()->get_value_index_neumann(
          x + 2 * direction(0),
          y + 2 * direction(1),
          z + 2 * direction(2)
          );

      return ( target == 255 );

    }

    /*----------------------------------------------------------------------*/

    const Scan& scan;

    // flag indicating of the normals should be flipped
    bool flip;

    // radius used for normal estimation
    double radius;

};

#endif
