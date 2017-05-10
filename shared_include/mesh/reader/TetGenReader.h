#ifndef __TETGEN_READER_H__
#define __TETGEN_READER_H__

#include <string>
#include <fstream>
#include <stdexcept>

#include <armadillo>

#include "mesh/reader/MeshReader.h"

/* class for reading files output by tetgen */
class TetGenReader : public MeshReader {

  public:

    TetGenReader() :
      MeshReader() {
      }

    /*-------------------------------------------------------------------------*/

    Mesh& read_mesh_from(const std::string& fileName) {

      // clear mesh
      this->mesh = Mesh();

      // create file name for node file
      std::string nodeFile = std::string(fileName + ".node");
      this->meshFile.open(nodeFile);

      if(this->meshFile.is_open() == false ) {
        throw std::runtime_error("Cannot open file " + nodeFile + ".");
      }

      read_vertex_data();

      this->meshFile.close()

      // create file name for facets file
      std::string facetFile = std::string(fileName + ".smesh");
      this->meshFile.open(facetFile);

      if(this->meshFile.is_open() == false ) {
        throw std::runtime_error("Cannot open file " + facetFile + ".");
      }

      read_face_data();

      this->meshFile.close();

      return this->mesh;

    }

    /*-------------------------------------------------------------------------*/

  private:

    /*-------------------------------------------------------------------------*/

    /* read vertex data */
    void read_vertex_data() {

      std::string line = getline();
      std::stringstream tmpStream(line);

      tmpStream >> this->vertexAmount;

      for( size_t i = 0; i < this->vertexAmount; ++i) {

        std::string line = getline();

        std::stringstream lineStream(line);

        int tmp;
        lineStream >> tmp;

        double x, y, z;

        lineStream >> x;
        lineStream >> y;
        lineStream >> z;

        this->vertices.push_back(arma::vec({x, y, z}));
      }

    }

    /*-------------------------------------------------------------------------*/

    void read_face( std::stringstream& stream) {

      std::vector<unsigned int> vertexIndices;

      size_t amount = 0;

      // read amount of vertices forming the face
      stream >> amount;

      if( stream.eof() || stream.fail() ) {
        throw_error("Problem reading face data");
      }

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

      if( !stream.eof() ) {
        throw_error("Too many vertex indices for face");
      }

      // store the face information
      this->faces.push_back(vertexIndices);


    }

    /*-------------------------------------------------------------------------*/

    void read_face_data() {

      std::string line;

      do{
        line = getline();
      }
      while(line != "# part 2: facet list.");

      line = getline();
      std::stringstream tmpStream(line);

      tmpStream >> this->faceAmount;

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
