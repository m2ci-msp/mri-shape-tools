#ifndef __PLY_READER_H__
#define __PLY_READER_H__

#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <regex>

#include <armadillo>

#include "mesh/reader/MeshReader.h"

/* class for reading ply files */
class PlyReader : public MeshReader {

public:

  PlyReader() :
    MeshReader(),
    vertexColors(this->mesh.get_vertex_colors()),
    vertexBoundaryMarkers(this->mesh.get_vertex_boundary_markers()),
    faceBoundaryMarkers(this->mesh.get_face_boundary_markers()) {
  }

  /*-------------------------------------------------------------------------*/

  Mesh& read_mesh_from(const std::string& fileName) {

    // clear mesh
    this->mesh = Mesh();

    this->meshFile.open(fileName);

    if(this->meshFile.is_open() == false ) {
      throw std::runtime_error("Cannot open file " + fileName + ".");
    }

    this->headerEnded = false;

    read_header_information();
    read_vertex_data();
    read_face_data();

    this->meshFile.close();

    return this->mesh;

  }

  /*-------------------------------------------------------------------------*/

private:

  /*-------------------------------------------------------------------------*/

  /* read needed header information of ply file */
  void read_header_information() {

    std::string read;

    // verify that we have a ply file
    read = getline();
    if( read != "ply" ) {
      throw std::runtime_error("File is no ply file.");
    }

    // check that it is in ASCII format
    read = getline();
    if( read != "format ascii 1.0" ) {
      throw std::runtime_error("Only ply files in ASCII format are supported.");
    }

    read_vertex_information();

    // face data is optional
    try{
      read_face_information();
    }
    catch(std::exception& e) {
      // problem reading face data -> set face amount to 0
      this->faceAmount = 0;
    }

    // reset line number
    this->lineNumber = 2;

    // move to the end of the header
    while( this->headerEnded == false) {
      this->headerEnded = (getline() == "end_header");
    }

  }

  /*-------------------------------------------------------------------------*/

  /* helper function that tries to find the amount of data belonging to the
   * specified element
   */
  int find_element_count( std::string elementName ) {

    // create regular expression
    std::regex elementFormat(
      "element[[:blank:]]+" +
      elementName +
      "[[:blank:]]([[:digit:]]+)"
      );
    std::smatch match;
    std::string read;

    // search the header for the wanted information
    do{
      read = getline();
      if( read == "end_header" || this->meshFile.eof() || this->meshFile.fail() ) {

        if( read == "end_header") {
          // record that header ended
          this->headerEnded = true;
        }

        throw std::runtime_error("Could not read " + elementName + " information.");
      }
    } while(! std::regex_match(read, match, elementFormat));

    return stoi(match[1]);
  }

  /*-------------------------------------------------------------------------*/

  /* read vertex amount and properties from header */
  void read_vertex_information() {

    // save current position in stream
    std::ios::pos_type pos = this->meshFile.tellg();

    this->vertexAmount = find_element_count(std::string("vertex"));

    read_vertex_property_list();

    // reset position
    this->meshFile.seekg(pos);

  }

  /*-------------------------------------------------------------------------*/

  /* read face amount from header */
  void read_face_information() {

    // save current position in stream
    std::ios::pos_type pos = this->meshFile.tellg();

    this->faceAmount = find_element_count(std::string("face"));

    getline();

    read_face_property_list();

    // reset position
    this->meshFile.seekg(pos);

  }

  /*-------------------------------------------------------------------------*/

  /* read vertex property list in the header */
  void read_vertex_property_list() {

    std::string tmp;
    size_t index = 0;

    // create regular expression for property
    std::regex propertyFormat(
      "property[[:blank:]]+(float|uchar)[[:blank:]]+([[:alpha:]]+)");
    std::smatch match;

    // extract all properties
    tmp = getline();

    while(std::regex_match(tmp, match, propertyFormat)) {
      std::string id = match[2];

      // save property name -> index correspondence
      this->vertexPropertyIndices[id] = index;
      ++index;

      this->vertexPropertyPresent[id] = true;

      tmp = getline();
    }

  }

  /*-------------------------------------------------------------------------*/

  /* read vertex property list in the header */
  void read_face_property_list() {

    std::string tmp;
    size_t index = 0;

    // create regular expression for property
    std::regex propertyFormat(
      "property[[:blank:]]+(float|uchar)[[:blank:]]+([[:alpha:]]+)");
    std::smatch match;

    // extract all properties
    tmp = getline();

    while(std::regex_match(tmp, match, propertyFormat)) {
      std::string id = match[2];

      // save property name -> index correspondence
      this->facePropertyIndices[id] = index;
      ++index;

      this->facePropertyPresent[id] = true;

      tmp = getline();
    }

  }

  /*-------------------------------------------------------------------------*/

  /* returns the index of the property with provided id */
  size_t get_vertex_property_index(
    const std::string propertyId
    ) const {

    return this->vertexPropertyIndices.at(propertyId);

  }

  /*-------------------------------------------------------------------------*/

  /* returns the index of the property with provided id */
  size_t get_face_property_index(
    const std::string propertyId
    ) const {

    return this->facePropertyIndices.at(propertyId);

  }


  /*-------------------------------------------------------------------------*/

  /* read vertex properties */
  std::vector<double> read_vertex_properties(std::stringstream& stream) {

    std::vector<double> properties;

    // read all properties for a vertex
    for( size_t j = 0; j < this->vertexPropertyIndices.size(); ++j ) {

      if( stream.eof() ) {
        throw_error("Problem reading vertex properties");
      }

      double property;
      stream >> property;

      properties.push_back(property);

    }

    if( stream.fail() ) {
      throw_error("Problem reading vertex properties");
    }

    /*  if( !stream.eof() ) {
        throw_error("Too much vertex properties");
        }
    */

    return properties;

  }

  /*-------------------------------------------------------------------------*/

  void read_vertex_position(const std::vector<double>& properties ) {

    const double x = properties.at(get_vertex_property_index("x"));
    const double y = properties.at(get_vertex_property_index("y"));
    const double z = properties.at(get_vertex_property_index("z"));

    this->vertices.push_back(arma::vec({x, y, z}));

  }

  /*-------------------------------------------------------------------------*/

  void read_vertex_normal(const std::vector<double>& properties ) {

    // normal information is optional
    if( this->vertexPropertyPresent["nx"] == false ) {
      return;
    }

    const double nx = properties.at(get_vertex_property_index("nx"));
    const double ny = properties.at(get_vertex_property_index("ny"));
    const double nz = properties.at(get_vertex_property_index("nz"));

    this->vertexNormals.push_back(arma::vec({nx, ny, nz}));

  }

  /*-------------------------------------------------------------------------*/

  void read_vertex_color(const std::vector<double>& properties ) {

    // color information is optional
    if( this->vertexPropertyPresent["red"] == false ) {
      return;
    }

    const double red = properties.at(get_vertex_property_index("red"));
    const double green = properties.at(get_vertex_property_index("green"));
    const double blue = properties.at(get_vertex_property_index("blue"));

    this->vertexColors.push_back(arma::vec({red, green, blue}));

  }

  /*-------------------------------------------------------------------------*/

  void read_vertex_boundary_marker(const std::vector<double>& properties) {

    // boundary information is optional
    if( this->vertexPropertyPresent["boundary"] == false ) {
      return;
    }

    const bool marker = properties.at(get_vertex_property_index("boundary"));
    this->vertexBoundaryMarkers.push_back(marker);

  }

  /*-------------------------------------------------------------------------*/


  /* read vertex data */
  void read_vertex_data() {

    for( size_t i = 0; i < this->vertexAmount; ++i) {

      std::string line = getline();

      std::stringstream lineStream(line);
      std::vector<double> properties = read_vertex_properties(lineStream);

      read_vertex_position(properties);
      read_vertex_normal(properties);
      read_vertex_color(properties);
      read_vertex_boundary_marker(properties);
    }

  }

  /*-------------------------------------------------------------------------*/

  void read_face_boundary_marker(const std::vector<double>& properties) {

    // boundary information is optional
    if( this->facePropertyPresent["boundary"] == false ) {
      return;
    }

    const bool marker = properties.at(get_face_property_index("boundary"));
    this->faceBoundaryMarkers.push_back(marker);

  }

  /*-------------------------------------------------------------------------*/

  /* read face properties */
  std::vector<double> read_face_properties(std::stringstream& stream) {

    std::vector<double> properties;

    // read all properties for a vertex
    for( size_t j = 0; j < this->facePropertyIndices.size(); ++j ) {

      if( stream.eof() ) {
        throw_error("Problem reading face properties");
      }

      double property;
      stream >> property;

      properties.push_back(property);

    }

    if( stream.fail() ) {
      throw_error("Problem reading face properties");
    }

    /*  if( !stream.eof() ) {
        throw_error("Too much vertex properties");
        }
    */

    return properties;

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

    std::vector<double> properties = read_face_properties(stream);

    read_face_boundary_marker(properties);

/*
  if( !stream.eof() ) {
  throw_error("Too many vertex indices for face");
  }
*/

    // store the face information
    this->faces.push_back(vertexIndices);


  }

  /*-------------------------------------------------------------------------*/

  void read_face_data() {

    for( size_t i = 0; i < this->faceAmount; ++i) {

      std::string line = getline();
      std::stringstream lineStream(line);

      read_face(lineStream);

    }

  }

  /*-------------------------------------------------------------------------*/

  std::vector<arma::vec>& vertexColors;
  std::vector<bool>& vertexBoundaryMarkers;
  std::vector<bool>& faceBoundaryMarkers;

  std::map<std::string, int> vertexPropertyIndices;
  std::map<std::string, int> facePropertyIndices;

  std::map<std::string, bool> vertexPropertyPresent;
  std::map<std::string, bool> facePropertyPresent;

  size_t vertexAmount;
  size_t faceAmount;

  bool headerEnded;

};
#endif
