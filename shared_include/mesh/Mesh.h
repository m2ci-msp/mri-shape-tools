#ifndef __MESH_H__
#define __MESH_H__

#include <stdexcept>
#include <vector>
#include <cfloat>

#include <armadillo>

/* class for basic storage of a face-vertex mesh */

class Mesh{
public:

  /*-------------------------------------------------------------------------*/
  // SETTERS
  /*-------------------------------------------------------------------------*/

  Mesh& set_vertices(const std::vector<arma::vec>& vertices) {

    this->vertices = vertices;
    return *this;

  }

  /*-------------------------------------------------------------------------*/

  Mesh& set_vertex_normals(const std::vector<arma::vec>& vertexNormals) {

    if( vertexNormals.size() != this->vertices.size() ) {
      throw std::runtime_error(
        "Vertex normal amount does not match vertex amount.");
    }

    this->vertexNormals = vertexNormals;
    return *this;
  }

  /*-------------------------------------------------------------------------*/

  Mesh& set_vertex_colors(const std::vector<arma::vec>& vertexColors) {

    if( vertexColors.size() != this->vertices.size() ) {
      throw std::runtime_error(
        "Vertex color amount does not match vertex amount.");
    }

    this->vertexColors = vertexColors;
    return *this;

  }

  /*-------------------------------------------------------------------------*/

  Mesh& set_vertex_boundary_markers(
    const std::vector<bool>& vertexBoundaryMarkers
    ) {

    if( vertexBoundaryMarkers.size() != this->vertices.size() ) {
      throw std::runtime_error(
        "Vertex boundary marker amount does not match vertex amount.");
    }

    this->vertexBoundaryMarkers = vertexBoundaryMarkers;
    return *this;

  }

  /*-------------------------------------------------------------------------*/

  Mesh& set_faces(const std::vector< std::vector<unsigned int> >& faces) {

    this->faces = faces;
    return *this;

  }

  /*-------------------------------------------------------------------------*/

  Mesh& set_face_boundary_markers(
    const std::vector<bool>& faceBoundaryMarkers
    ) {

    if( faceBoundaryMarkers.size() != this->faces.size() ) {
      throw std::runtime_error(
        "Face boundary marker amount does not match face amount.");
    }

    this->faceBoundaryMarkers = faceBoundaryMarkers;
    return *this;

  }

  /*-------------------------------------------------------------------------*/
  // ADDERS
  /*-------------------------------------------------------------------------*/

  Mesh& add_vertices(const std::vector<arma::vec>& vertices) {

    this->vertices.insert(
      this->vertices.end(), vertices.begin(), vertices.end());

    return *this;

  }

  /*-------------------------------------------------------------------------*/

  Mesh& add_vertex_normals(const std::vector<arma::vec>& vertexNormals) {

    this->vertexNormals.insert(
      this->vertexNormals.end(), vertexNormals.begin(), vertexNormals.end());

    return *this;

  }

  /*-------------------------------------------------------------------------*/


  Mesh& add_faces(const std::vector< std::vector<unsigned int> >& faces) {

    this->faces.insert(
      this->faces.end(), faces.begin(), faces.end());

    return *this;

  }

  /*-------------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------*/
  // GETTERS
  /*-------------------------------------------------------------------------*/

  std::vector<arma::vec>& get_vertices() {
    return this->vertices;
  }

  /*-------------------------------------------------------------------------*/

  // const version
  const std::vector<arma::vec>& get_vertices() const {
    return this->vertices;
  }


  /*-------------------------------------------------------------------------*/

  std::vector<arma::vec>& get_vertex_normals() {
    return this->vertexNormals;
  }

  /*-------------------------------------------------------------------------*/

  // const version
  const std::vector<arma::vec>& get_vertex_normals() const {
    return this->vertexNormals;
  }

  /*-------------------------------------------------------------------------*/

  std::vector<arma::vec>& get_vertex_colors() {
    return this->vertexColors;
  }

  /*-------------------------------------------------------------------------*/

  // const version
  const std::vector<arma::vec>& get_vertex_colors() const {
    return this->vertexColors;
  }

  /*-------------------------------------------------------------------------*/

  std::vector<bool>& get_vertex_boundary_markers() {
    return this->vertexBoundaryMarkers;
  }

  /*-------------------------------------------------------------------------*/

  // const version
  const std::vector<bool>& get_vertex_boundary_markers() const {
    return this->vertexBoundaryMarkers;
  }

  /*-------------------------------------------------------------------------*/


  std::vector< std::vector<unsigned int> >& get_faces() {
    return this->faces;
  }

  /*-------------------------------------------------------------------------*/

  // const version
  const std::vector< std::vector<unsigned int> >& get_faces() const {
    return this->faces;
  }

  /*-------------------------------------------------------------------------*/

  std::vector<bool>& get_face_boundary_markers() {
    return this->faceBoundaryMarkers;
  }

  /*-------------------------------------------------------------------------*/

  // const version
  const std::vector<bool> & get_face_boundary_markers() const {
    return this->faceBoundaryMarkers;
  }

  /*-------------------------------------------------------------------------*/

  const arma::vec get_vertex(const int& id) {
    return this->vertices.at(id);
  }

  /*-------------------------------------------------------------------------*/

  // const version
  const arma::vec get_vertex(const int& id) const {
    return this->vertices.at(id);
  }

  /*-------------------------------------------------------------------------*/

  const arma::vec get_normal(const int& id) {
    return this->vertexNormals.at(id);
  }

  /*-------------------------------------------------------------------------*/

  // const version
  const arma::vec get_normal(const int& id) const {
    return this->vertexNormals.at(id);
  }

  /*-------------------------------------------------------------------------*/

  size_t get_vertex_amount() const {
    return this->vertices.size();
  }

  /*-------------------------------------------------------------------------*/

  size_t get_face_amount() const {
    return this->faces.size();
  }

  /*-------------------------------------------------------------------------*/

  const arma::vec get_center() const {

    arma::vec mean = arma::zeros<arma::vec>(3);

    for( const auto& point: this->vertices) {
      mean += point;
    }

    return mean / this->vertices.size();
  }

  /*-------------------------------------------------------------------------*/

  void get_bounding_box(arma::vec& min, arma::vec& max) const {

    min = arma::vec({DBL_MAX, DBL_MAX, DBL_MAX});
    max = -min;

    for(const arma::vec& vertex: this->vertices) {

      for(int i = 0; i < 3; ++i) {

        min(i) = (min(i) > vertex(i))? vertex(i): min(i);
        max(i) = (max(i) < vertex(i))? vertex(i): max(i);

      }

    }

  }

  /*-------------------------------------------------------------------------*/

  bool has_normals() const {
    return this->vertexNormals.size() == this->vertices.size();
  }

  /*-------------------------------------------------------------------------*/

  bool has_faces() const {
    return this->faces.size() != 0;
  }

  /*-------------------------------------------------------------------------*/


protected:

  std::vector<arma::vec> vertices;
  std::vector<arma::vec> vertexNormals;
  std::vector<arma::vec> vertexColors;

  std::vector<bool> vertexBoundaryMarkers;
  std::vector<bool> faceBoundaryMarkers;

  // face is represented as a list of vertex indices
  std::vector< std::vector<unsigned int> > faces;

  size_t vertexAmount;
  size_t faceAmount;

};
#endif
