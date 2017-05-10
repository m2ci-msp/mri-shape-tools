#ifndef __MDL_READER_H__
#define __MDL_READER_H__

#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <regex>

#include <armadillo>

#include "mesh/reader/MeshReader.h"

/* class for reading ply files */
class MdlReader : public MeshReader {

  public:

    MdlReader() :
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
      read_face_data();

      this->meshFile.close();

      return this->mesh;

    }

    /*-------------------------------------------------------------------------*/

  private:


    /*-------------------------------------------------------------------------*/

    int get_amount() {
      std::string amountLine = getline();
      std::stringstream lineStream(amountLine);
      int amount;
      lineStream >> amount;

      return amount;
    }

    /*-------------------------------------------------------------------------*/

    /* read vertex data */
    void read_vertex_data() {

      while(getline() != "[Vertices, ARRAY1<POINT3D>]") {
      }

      this->vertexAmount = get_amount();


      for( size_t i = 0; i < this->vertexAmount; ++i) {

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

      arma::vec vertex({x, z, y});

      this->vertices.push_back(vertex);

    }

    /*-------------------------------------------------------------------------*/
 
    void read_face( std::stringstream& stream) {

      std::vector<unsigned int> vertexIndices;

      size_t amount = 3;

      // read the vertex indices
      for( size_t j = 0; j < amount; ++j ) {

        if( stream.eof() ) {
          throw_error(
              "Problem reading vertex index " +
              std::to_string(j) +
              " for face");
        }

        size_t index;
        stream >> index;

        if( stream.fail() ) {
          throw_error(
              "Problem reading vertex index " +
              std::to_string(j) +
              " for face");
        }

        vertexIndices.push_back(index);

      }

      // store the face information
      this->faces.push_back(vertexIndices);


    }

    /*-------------------------------------------------------------------------*/

    void read_face_data() {

      while(getline() != "[Triangles, ARRAY1<STRING>]") {
      }

      this->faceAmount = get_amount();

      for( size_t i = 0; i < this->faceAmount; ++i) {

        std::string line = getline();
        std::stringstream lineStream(line);

        read_face(lineStream);

      }

    }

    /*-------------------------------------------------------------------------*/

    size_t vertexAmount;
    size_t faceAmount;

};
#endif
