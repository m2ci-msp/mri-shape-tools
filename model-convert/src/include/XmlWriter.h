#ifndef __XML_WRITER_H__
#define __XML_WRITER_H__

#include <mesh/writer/MeshWriter.h>

#include <armadillo>

class XmlWriter : public MeshWriter {

  public:

    XmlWriter(const Mesh& mesh) :
      MeshWriter(mesh) {
      }

    /*-------------------------------------------------------------------------*/

    void write_mesh_to(
        const std::string& fileName
        ) {

      this->meshFile.open(fileName);

      open_xml();

      write_face_data();
      write_vertex_data();

      close_xml();
    }


    /*-------------------------------------------------------------------------*/

  private:

    void open_xml() {

      this->meshFile << "<xml3d xmlns=\"http://www.xml3d.org/2009/xml3d\">" << std::endl
                     << "<defs>" << std::endl
                     << "<data id=\"mesh-data\">" << std::endl;
    }

    /*-------------------------------------------------------------------------*/

    void close_xml() {

      this->meshFile << "</data>" << std::endl
                     << "</defs>" << std::endl
                     << "</xml3d>" << std::endl;
    }

    /*-------------------------------------------------------------------------*/

    void write_vertex_data() {

      this->meshFile << "<float3 name=\"position\">" << std::endl;
      for( auto vertex: this->vertices) {

        this->meshFile << vertex(0) << " "
                       << vertex(1) << " "
                       << vertex(2) << " ";

      }

      this->meshFile << std::endl
                    << "</float3>" << std::endl;
    }

    /*-------------------------------------------------------------------------*/

    void write_face_data() {

      this->meshFile << "<int name=\"index\">" << std::endl;

      for( size_t i = 0; i < this->faces.size(); ++i) {

        std::vector<unsigned int> vertexIndices = this->faces.at(i);

        for( auto index: vertexIndices) {

          this->meshFile << index << " ";

        }

      }


    this->meshFile << std::endl
                   << "</int>" << std::endl;
    }


    /*-------------------------------------------------------------------------*/

};
#endif
