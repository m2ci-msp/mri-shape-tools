#ifndef __MESH_WRITER_H__
#define __MESH_WRITER_H__

#include <string>
#include <fstream>
#include <armadillo>

#include "mesh/Mesh.h"


class MeshWriter{
  
  public:

    MeshWriter(const Mesh& mesh) : 
      // initialize references
      vertices(mesh.get_vertices()),
      vertexNormals(mesh.get_vertex_normals()),
      vertexBoundaryMarkers(mesh.get_vertex_boundary_markers()),
      faces(mesh.get_faces()),
      faceBoundaryMarkers(mesh.get_face_boundary_markers()) {
      }

    /*-------------------------------------------------------------------------*/

    void set_comments(const std::vector<std::string>& comments) {
      this->comments = comments;
    }

    /*-------------------------------------------------------------------------*/

    virtual void write_mesh_to(const std::string& fileName) = 0; 


  protected:

    const std::vector<arma::vec>& vertices;
    const std::vector<arma::vec>& vertexNormals;
    const std::vector<bool>& vertexBoundaryMarkers;
    const std::vector< std::vector<unsigned int> > faces;
    const std::vector<bool> faceBoundaryMarkers;


    std::vector<std::string> comments;

    std::ofstream meshFile;
};
#endif
