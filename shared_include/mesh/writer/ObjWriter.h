#ifndef __OBJ_WRITER_H__
#define __OBJ_WRITER_H__

#include <string>
#include <vector>

#include <armadillo>

#include "mesh/writer/MeshWriter.h"

/* class for writing obj files */
class ObjWriter : public MeshWriter {

  public:

    ObjWriter(const Mesh& mesh) :
      MeshWriter(mesh) {
    }

    /*-------------------------------------------------------------------------*/

    void write_mesh_to(
      const std::string& fileName
      ) {

      this->meshFile.open(fileName);

      write_vertex_data();
      write_face_data();

      this->meshFile.close();

    }

    /*-------------------------------------------------------------------------*/

  private:

    /*-------------------------------------------------------------------------*/

    void write_comments() {

      for( const auto& comment: this->comments ) {
        this->meshFile << "# " << comment << std::endl;
      }

    }

    /*-------------------------------------------------------------------------*/

    void write_vertex(const size_t index) {

      arma::vec v = this->vertices.at(index);

      this->meshFile << "v ";
      this->meshFile << v(0) << " ";
      this->meshFile << v(1) << " ";
      this->meshFile << v(2) << std::endl;

    }

    /*-------------------------------------------------------------------------*/

    void write_vertex_normal(const size_t index) {

      if( this->vertexNormals.empty() == false ) {

        const arma::vec vn = this->vertexNormals.at(index);
        this->meshFile << "vn ";
        this->meshFile << vn(0) << " ";
        this->meshFile << vn(1) << " ";
        this->meshFile << vn(2) << std::endl;
      }

    }
 
    /*-------------------------------------------------------------------------*/

    void write_vertex_data() {

      for( size_t i = 0; i < this->vertices.size(); ++i) {

        write_vertex(i);
        write_vertex_normal(i);

      }

    }

    /*-------------------------------------------------------------------------*/

    void write_face_data() {

      for( size_t i = 0; i < this->faces.size(); ++i) {

        std::vector<unsigned int> vertexIndices = this->faces.at(i);

        this->meshFile << "f ";

        for( auto index: vertexIndices) {

          // increment, in OBJ vertex indices start at 1
          ++index;

          this->meshFile << " " << index;

          if( this->vertexNormals.empty() == false ) {
            this->meshFile << "//" << index;
          }

        }

        this->meshFile << std::endl;
      }

    }

    /*-------------------------------------------------------------------------*/

};
#endif
