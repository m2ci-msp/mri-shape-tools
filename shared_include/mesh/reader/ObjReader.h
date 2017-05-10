#ifndef __OBJ_READER_H__
#define __OBJ_READER_H__

#include <string>
#include <sstream>

#include <armadillo>

#include "mesh/Mesh.h"
#include "mesh/reader/MeshReader.h"

class ObjReader : public MeshReader {

  public:

    ObjReader() : MeshReader() {
    }

    /*-------------------------------------------------------------------------*/

    const Mesh& read_mesh_from( const std::string& fileName )  {

      // clear mesh
      this->mesh = Mesh();

      this->meshFile.open(fileName);

      if(this->meshFile.is_open() == false ) {
        throw std::runtime_error("Cannot open file " + fileName + ".");
      }

      this->lineNumber = 0;

      read_data();

      this->meshFile.close();

      return this->mesh;
    }

    /*-------------------------------------------------------------------------*/

  private:

    void read_vertex_position(std::stringstream& stream ) {

      double x, y, z;

      // only read coordinates
      stream >> x >> y >> z;

      this->vertices.push_back(arma::vec({x, y, z}));

      if( stream.fail() ) {
        throw_error("Problem reading vertex position");
      }

    }

    /*-------------------------------------------------------------------------*/

    void read_vertex_normal(std::stringstream& stream ) {

      double nx, ny, nz;

      stream >> nx >> ny >> nz;

      this->vertexNormals.push_back(arma::vec({nx, ny, nz}));

      if( stream.fail() ) {
        throw_error("Problem reading vertex normal");
      }

      if( !stream.eof() ) {
        throw_error("Too much components for vertex normal");
      }

    }

    /*-------------------------------------------------------------------------*/

    void read_face(std::stringstream& stream ) {

      std::vector<unsigned int> vertexIndices;

      while( !stream.fail() && !stream.eof() ) {
        int vertexId = 0;

        stream >> vertexId;

        // decrement, in OBJ vertex indices start at 1
        --vertexId;

        vertexIndices.push_back(vertexId);

        // discard rest if present
        if( !stream.eof() && stream.peek() == '/') {
          std::string tmp;
          stream >> tmp;
        }

      }

      if( stream.fail() ) {
        throw_error("Problem reading face data");
      }

      this->faces.push_back(vertexIndices);

    }


    /*-------------------------------------------------------------------------*/

    /* read data */
    void read_data() {

      while( ! this->meshFile.fail() && ! this->meshFile.eof() ) 
      {

        std::string line = getline();

        std::string id;
        std::stringstream lineStream(line);
        lineStream >> id;

        if( id == "v") {
          read_vertex_position(lineStream);
        }

        if( id == "vn") {
          read_vertex_normal(lineStream);
        }

        if( id == "f") {
          read_face(lineStream);
        }


      } // while

    }

    /*-------------------------------------------------------------------------*/

};
#endif
