#ifndef __MAT_READER_H__
#define __MAT_READER_H__

#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <regex>

#include <armadillo>

#include "mesh/reader/MeshReader.h"

/* class for reading mat files */
class MatReader : public MeshReader {

  public:

    MatReader() :
      MeshReader() {
      }

    /*-------------------------------------------------------------------------*/

    Mesh& read_mesh_from(const std::string& fileName) {

      // clear mesh
      this->mesh = Mesh();

      this->meshFile.open(fileName);

      if(this->meshFile.is_open() == false ) {
        throw std::runtime_error("Cannot open file " + fileName + ".");
      }

      read_vertex_data();

      this->meshFile.close();

      return this->mesh;

    }

    /*-------------------------------------------------------------------------*/

  private:


    /*-------------------------------------------------------------------------*/

    /* read vertex data */
    void read_vertex_data() {

      while(!this->meshFile.eof()) {
        std::string line = getline();

        std::stringstream lineStream(line);
        read_vertex(lineStream);
      }

    }

    /*-------------------------------------------------------------------------*/
    void read_vertex( std::stringstream& stream) {

      double x, y, z;

      stream >> x;
      stream >> y;
      stream >> z;

      arma::vec vertex({x, 240-z, 24+y});

      this->vertices.push_back(vertex);

    }

    /*-------------------------------------------------------------------------*/
 
};
#endif
