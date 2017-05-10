#ifndef __PLY_WRITER_H__
#define __PLY_WRITER_H__

#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <regex>

#include <armadillo>

#include "mesh/writer/MeshWriter.h"

/* class for writing ply files */
class PlyWriter : public MeshWriter {
  public:

    PlyWriter(const Mesh& mesh) :
      MeshWriter(mesh),
      vertexColors(mesh.get_vertex_colors()) {
    }

    /*-------------------------------------------------------------------------*/

    void write_mesh_to( const std::string& fileName) {

      this->meshFile.open(fileName);

      write_header_information();
      write_vertex_data();
      write_face_data();

      this->meshFile.close();

      return;
    }

    /*-------------------------------------------------------------------------*/

  private:

    /*-------------------------------------------------------------------------*/

    void write_header_information() {

      std::string read; 

      this->meshFile << "ply" << std::endl;
      this->meshFile << "format ascii 1.0" << std::endl;

      write_comments();
      write_vertex_information();
      write_face_information();

      this->meshFile << "end_header" << std::endl;

    }

    /*-------------------------------------------------------------------------*/

    void write_comments() {

      for( const auto& comment: this->comments ) {
        this->meshFile << "comment " << comment << std::endl;
      }

    }

    /*-------------------------------------------------------------------------*/

    /* write vertex amount and property list to header */
    void write_vertex_information() {

      this->meshFile << "element vertex "  << this->vertices.size() << std::endl;
      this->meshFile << "property float x" << std::endl;
      this->meshFile << "property float y" << std::endl;
      this->meshFile << "property float z" << std::endl;

      if( this->vertexNormals.empty() == false ) {
        this->meshFile << "property float nx" << std::endl;
        this->meshFile << "property float ny" << std::endl;
        this->meshFile << "property float nz" << std::endl;
      }

      if( this->vertexColors.empty() == false ) {
        this->meshFile << "property uchar red" << std::endl;
        this->meshFile << "property uchar green" << std::endl;
        this->meshFile << "property uchar blue" << std::endl;
      }

      if( this->vertexBoundaryMarkers.empty() == false) {
        this->meshFile << "property float boundary" << std::endl;
      }

    }

    /*-------------------------------------------------------------------------*/

    /* write face information to header */
    void write_face_information() {

      if( this->faces.size() > 0 ) {
        this->meshFile << "element face " << this->faces.size() << std::endl;
        this->meshFile << "property list uchar uint vertex_indices" << std::endl;

        if( this->faceBoundaryMarkers.empty() == false) {
          this->meshFile << "property float boundary" << std::endl;
        }


      }

    }

    /*-------------------------------------------------------------------------*/

    void write_vertex(const size_t index) {

      // write coordinate information
      const arma::vec v = this->vertices.at(index);

      this->meshFile << v(0) << " ";
      this->meshFile << v(1) << " ";
      this->meshFile << v(2);

    }

    /*-------------------------------------------------------------------------*/

    void write_vertex_normal(const size_t index) {

      if( this->vertexNormals.empty() == false ) {
        const arma::vec vn = this->vertexNormals.at(index);

        this->meshFile << " ";
        this->meshFile << vn(0) << " ";
        this->meshFile << vn(1) << " ";
        this->meshFile << vn(2);
      }

    }
 
    /*-------------------------------------------------------------------------*/

    void write_vertex_color(const size_t index) {

      if( this->vertexColors.empty() == false ) {
        const arma::vec vc = this->vertexColors.at(index);

        this->meshFile << " ";
        this->meshFile << vc(0) << " ";
        this->meshFile << vc(1) << " ";
        this->meshFile << vc(2);
      }

    }

    /*-------------------------------------------------------------------------*/

    void write_vertex_boundary_marker(const size_t index) {

      if( this->vertexBoundaryMarkers.empty() == false ) {
        const bool marker = this->vertexBoundaryMarkers.at(index);

        this->meshFile << " ";
        this->meshFile << marker;
      }

    }


    /*-------------------------------------------------------------------------*/

    void write_vertex_data() {

      for( size_t i = 0; i < this->vertices.size(); ++i) {

        write_vertex(i);
        write_vertex_normal(i);
        write_vertex_color(i);
        write_vertex_boundary_marker(i);

        this->meshFile << std::endl;

      }
    }

    /*-------------------------------------------------------------------------*/

    void write_face_data() {

      for( size_t i = 0; i < this->faces.size(); ++i) {

        const std::vector<unsigned int>& vertexIndices = this->faces.at(i);

        const size_t amount = vertexIndices.size();

        this->meshFile << amount;

        for( const auto& index: vertexIndices) {
          this->meshFile << " " << index;
        }

        if( this->faceBoundaryMarkers.empty() == false ) {
          const bool marker = this->faceBoundaryMarkers.at(i);

          this->meshFile << " ";
          this->meshFile << marker;
        }

        this->meshFile << std::endl;
      }


    }

    /*-------------------------------------------------------------------------*/

    const std::vector<arma::vec>& vertexColors;


};
#endif
