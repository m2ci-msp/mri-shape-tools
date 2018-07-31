#ifndef __CREATE_POINT_CLOUD_H__
#define __CREATE_POINT_CLOUD_H__

#include "image/Image.h"
#include "mesh/Mesh.h"
#include "image/filter/diffusion/StructureTensorField.h"

class CreatePointCloud{

public:

  /*----------------------------------------------------------------------*/

  CreatePointCloud(
                   const ImageData& imageData,
                   const DiffusionSettings& settings
                   ) :

    data(imageData),
    access(data),
    info(data),
    structureTensors(data, settings) {

    // make boundary large enough
    ImageBoundary(this->data).change(2, 2, 2);
    ImageMirror(this->data).all();
    this->flip = false;

  }

  /*----------------------------------------------------------------------*/

  CreatePointCloud& set_flip(const bool& flip) {

    this->flip = flip;

    return *this;
  }

  /*----------------------------------------------------------------------*/

  Mesh create_point_cloud() {

    this->structureTensors.update();

    Mesh pointCloud;

    std::vector<arma::vec> vertices;
    std::vector<arma::vec> normals;

    const int nx = this->info.get_nx();
    const int ny = this->info.get_ny();
    const int nz = this->info.get_nz();

    const double hx = this->info.get_hx();
    const double hy = this->info.get_hy();
    const double hz = this->info.get_hz();

    const arma::vec origin({
        this->info.get_origin_x(),
          this->info.get_origin_y(),
          this->info.get_origin_z()
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

    if( this->access.at_grid(x, y, z) != 255 ) {

      return false;

    }

    return (
            this->access.at_grid( x - 1, y    , z    ) != 255 ||
            this->access.at_grid( x + 1, y    , z    ) != 255 ||
            this->access.at_grid( x    , y - 1, z    ) != 255 ||
            this->access.at_grid( x    , y + 1, z    ) != 255 ||
            this->access.at_grid( x    , y    , z - 1) != 255 ||
            this->access.at_grid( x    , y    , z + 1) != 255
            );

  }

  /*----------------------------------------------------------------------*/

  arma::vec derive_normal(
                          const int& x,
                          const int& y,
                          const int& z
                          ) {

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

    const double& j11 = this->structureTensors.J11.at_grid(x, y, z);
    const double& j22 = this->structureTensors.J22.at_grid(x, y, z);
    const double& j33 = this->structureTensors.J33.at_grid(x, y, z);
    const double& j12 = this->structureTensors.J12.at_grid(x, y, z);
    const double& j13 = this->structureTensors.J13.at_grid(x, y, z);
    const double& j23 = this->structureTensors.J23.at_grid(x, y, z);

    arma::mat J({
        { j11, j12, j13 },
          { j12, j22, j23 },
            { j13, j23, j33 }
      } );

    arma::vec eigval;
    arma::mat eigvec;

    arma::eig_sym(eigval, eigvec, J, "std");

    const arma::vec direction = eigvec.col(2);

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

    const double target = this->access.at_grid(
                                               x + 2 * direction(0),
                                               y + 2 * direction(1),
                                               z + 2 * direction(2)
                                               );

    return ( target == 255 );

  }

  /*----------------------------------------------------------------------*/


  ImageData data;
  ImageAccess access;
  ImageInfo info;
  StructureTensorField structureTensors;

  // flag indicating of the normals should be flipped
  bool flip;

};

#endif
