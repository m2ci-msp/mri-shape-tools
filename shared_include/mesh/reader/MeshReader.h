#ifndef __MESH_READER_H__
#define __MESH_READER_H__

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "mesh/Mesh.h"

/* base class for a mesh reader */
class MeshReader {

  public:

    MeshReader() :
      // initialize references
      vertices(this->mesh.get_vertices()),
      vertexNormals(this->mesh.get_vertex_normals()),
      faces(this->mesh.get_faces()) {
      }

    virtual const Mesh& read_mesh_from( const std::string& ) = 0;

  protected:

    /*-------------------------------------------------------------------------*/

    /* helper function that reads a line and turns the result into a string */
    const std::string getline() {

      std::string read;
      std::getline(this->meshFile, read);

      ++this->lineNumber;

      return read;
    }

    /*-------------------------------------------------------------------------*/

    void throw_error(const std::string what) const {
      throw std::runtime_error(
        what + " at line " + std::to_string(this->lineNumber)
      );
    }

    /*-------------------------------------------------------------------------*/

    Mesh mesh;

    std::vector<arma::vec>& vertices;
    std::vector<arma::vec>& vertexNormals;
    std::vector< std::vector<unsigned int> >& faces;

    size_t lineNumber;
    std::ifstream meshFile;


};
#endif
