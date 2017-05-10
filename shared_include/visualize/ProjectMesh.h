#ifndef __PROJECT_MESH_H__
#define __PROJECT_MESH_H__

#include "mesh/Mesh.h"
#include "scan/Scan.h"

class ProjectMesh{

  public:

    ProjectMesh(const Mesh& mesh, const Scan& scan, int sampleAmount = 5) :
      mesh(mesh), scan(scan), sampleAmount(sampleAmount) {

        const double hx = scan.data()->get_hx();
        const double hy = scan.data()->get_hy();
        const double hz = scan.data()->get_hz();

        this->scanOrigin = arma::vec( {
            scan.data()->get_org_x() * hx,
            scan.data()->get_org_y() * hy,
            scan.data()->get_org_z() * hz
            });

      }

    /*------------------------------------------------------------------------*/

    Scan get_projected_mesh() {

      project_mesh_to_scan();
      return this->scan;

    }

    /*------------------------------------------------------------------------*/

  private:

    /*------------------------------------------------------------------------*/

    void project_mesh_to_scan() {

      for( const auto& face: this->mesh.get_faces() ) {
        draw_face(face);
      }

    }

    /*------------------------------------------------------------------------*/

    void draw_face(const std::vector<unsigned int>& face) {

      // iterate over the edges between the vertices
      for( size_t i = 0; i < face.size() - 1; ++i) {

        arma::vec start = this->mesh.get_vertices().at(face.at(i    ));
        arma::vec end   = this->mesh.get_vertices().at(face.at(i + 1));

        draw_edge(start, end);

      }

      // create edge between starting vertex and end vertex 
      arma::vec start = this->mesh.get_vertices().at(face.at(0    ));
      arma::vec end   = this->mesh.get_vertices().at(face.at(face.size() - 1));

      draw_edge(start, end);

    }

    /*------------------------------------------------------------------------*/

    void draw_edge(arma::vec start, arma::vec end) {

      // compute normalized direction vector representing the edge
      const arma::vec direction = arma::normalise(end - start);

      // derive sample distance on edge
      const double sampleDistance =
        arma::norm(end - start) / this->sampleAmount;

      // sample edge
      for(int i = 0; i <= this->sampleAmount; ++i) {

        // sample point on edge and shift to origin of scan
        arma::vec coordinate =
          start + i * sampleDistance * direction - this->scanOrigin;

        // set voxel color to maximum at computed coordinate
        this->scan.access()->set_value_coordinate(
            coordinate,
            this->scan.data()->get_max()
            );
      } // end for

    } // end draw_edge

    /*------------------------------------------------------------------------*/

    Mesh mesh;
    Scan scan;

    double sampleAmount;

    arma::vec scanOrigin;

    /*------------------------------------------------------------------------*/
};
#endif
