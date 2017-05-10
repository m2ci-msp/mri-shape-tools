#ifndef __MESH_IO_H__
#define __MESH_IO_H__

#include <string>
#include <stdexcept>
#include <regex>

#include "mesh/reader/MeshReader.h"
#include "mesh/reader/ObjReader.h"
#include "mesh/reader/PlyReader.h"
#include "mesh/reader/MdlReader.h"
#include "mesh/reader/MatReader.h"

#include "mesh/writer/MeshWriter.h"
#include "mesh/writer/ObjWriter.h"
#include "mesh/writer/PlyWriter.h"



class MeshIO{
  public:

    /*---------------------------------------------------------------------------*/

    static Mesh read(const std::string& file) {
      
      std::regex regEx("[.]([[:alpha:]]+)$");
      std::smatch match;
      std::string extension;

      std::regex_search(file, match, regEx);

      extension = match[1];

      if( extension == "ply" ) {
        PlyReader reader;
        return reader.read_mesh_from(file);
      }
      else if( extension == "obj" ) {
        ObjReader reader;
        return reader.read_mesh_from(file);
      }
      else if( extension == "mdl" ) {
        MdlReader reader;
        return reader.read_mesh_from(file);
      }
      else if( extension == "mat" ) {
        MatReader reader;
        return reader.read_mesh_from(file);
      }
      else {
        throw std::runtime_error("Mesh format " + extension + " not supported.");
      }

    }

    /*---------------------------------------------------------------------------*/

    static void write(const Mesh& mesh, const std::string& file) {
      
      std::regex regEx("[.]([[:alpha:]]+)$");
      std::smatch match;
      std::string extension;

      std::regex_search(file, match, regEx);

      extension = match[1];

      if( extension == "ply" ) {
        PlyWriter writer(mesh);
        writer.write_mesh_to(file);
 
      }
      else if( extension == "obj" ) {
        ObjWriter writer(mesh);
        writer.write_mesh_to(file);
 
      }
      else {
        throw std::runtime_error("Mesh format " + extension + " not supported.");
      }

    }

    /*---------------------------------------------------------------------------*/


  private:
    MeshIO() {
    }

};
#endif
