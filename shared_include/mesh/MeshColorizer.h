#ifndef __MESH_COLORIZER_H__
#define __MESH_COLORIZER_H__

#include "mesh/Mesh.h"
#include "image/Image.h"

class MeshColorizer {

  public:

    /* color the vertices by using the values of a volume scan */
    static void colorize(Mesh& mesh, const Image& scan) {

      auto& colors = mesh.get_vertex_colors();
      // erase old colors
      colors.clear();

      auto vertices = mesh.get_vertices();

      // color the vertex according to the scan voxel it is located in
      for( const arma::vec& vertex: vertices) {

        const double& color = scan.access().at_coordinate(vertex(0), vertex(1), vertex(2));
        colors.push_back(arma::vec({color, color, color}));

      }

    }
};
#endif
