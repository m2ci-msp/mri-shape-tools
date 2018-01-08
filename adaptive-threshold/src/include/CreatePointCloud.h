#ifndef __CREATE_POINT_CLOUD_H__
#define __CREATE_POINT_CLOUD_H__

#include "image/Image.h"
#include "mesh/Mesh.h"

class CreatePointCloud{

  public:

    /*----------------------------------------------------------------------*/

    CreatePointCloud(Image& scan) :

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

      const int nx = this->scan.info().get_nx();
      const int ny = this->scan.info().get_ny();
      const int nz = this->scan.info().get_nz();

      const double hx = this->scan.info().get_hx();
      const double hy = this->scan.info().get_hy();
      const double hz = this->scan.info().get_hz();

      const int boundary = this->radius + 1;

      this->scan.boundary().change(boundary, boundary, boundary);
      this->scan.mirror().all();

      const arma::vec origin({
          this->scan.info().get_origin_x(),
          this->scan.info().get_origin_y(),
          this->scan.info().get_origin_z()
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
     * one of its 6 neighbors does not belong to the surface class
     */
    bool is_surface_point(
        const int& x,
        const int& y,
        const int& z
        ) const {

      if( this->scan.access().at_grid(x, y, z) != 255 ) {

        return false;

      }

      return (
              this->scan.access().at_grid( x - 1, y    , z    ) != 255 ||
              this->scan.access().at_grid( x + 1, y    , z    ) != 255 ||
              this->scan.access().at_grid( x    , y - 1, z    ) != 255 ||
              this->scan.access().at_grid( x    , y + 1, z    ) != 255 ||
              this->scan.access().at_grid( x    , y    , z - 1) != 255 ||
              this->scan.access().at_grid( x    , y    , z + 1) != 255
        );

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

        arma::vec central = this->scan.calculus().gradient(x, y, z);
        return arma::normalise(central);

      }

      // derive subspace of local principal gradient directions
      const arma::mat structureTensor =

        this->scan.calculus().region_structure_tensor(

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

      const double target = this->scan.access().at_grid(
          x + 2 * direction(0),
          y + 2 * direction(1),
          z + 2 * direction(2)
          );

      return ( target == 255 );

    }

    /*----------------------------------------------------------------------*/

    Image& scan;

    // flag indicating of the normals should be flipped
    bool flip;

    // radius used for normal estimation
    double radius;

};

#endif
