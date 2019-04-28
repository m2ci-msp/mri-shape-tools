#ifndef __MESH_TRUNCATOR_H__
#define __MESH_TRUNCATOR_H__

#include <map>
#include <set>
#include <vector>

#include <armadillo>

#include "mesh/Mesh.h"


class MeshTruncator{

private:

  Mesh originalMesh;
  Mesh truncatedMesh;

  std::set<int> subset;

  std::map<unsigned int, unsigned int> indexMap;

public:

  MeshTruncator(const Mesh& mesh) : originalMesh(mesh) {

  }

  Mesh to(const std::set<int>& subset) {

    this->truncatedMesh = this->originalMesh;

    this->subset = subset;

    create_index_map();

    truncate_faces();

    truncate_vertices();

    return this->truncatedMesh;

  }

private:

  void create_index_map() {

    this->indexMap.clear();

    unsigned int index = 0;

    for(const unsigned int& vertexId: this->subset) {

      this->indexMap[vertexId] = index;
      ++index;

    }

  }

  void truncate_faces() {

    std::vector< std::vector<unsigned int> > faces = this->originalMesh.get_faces();

    std::vector< std::vector<unsigned int> > newFaces;

    for(const auto& face: faces) {

      if( vertices_present_for_face(face) == true) {

        newFaces.push_back( build_face_with_updated_indices(face) );

      }

    }

    this->truncatedMesh.set_faces(newFaces);

  }

  bool vertices_present_for_face(const std::vector<unsigned int>& face) const {

    for(const unsigned int& vertexId: face) {

      if(this->subset.count(vertexId) == 0 ) {

        return false;

      }

    }

    return true;

  }

  std::vector<unsigned int> build_face_with_updated_indices(const std::vector<unsigned int> face) const {

    std::vector<unsigned int> newFace;

    for(const unsigned int& index: face) {

      newFace.push_back( this->indexMap.at(index) );

    }

    return newFace;

  }

  void truncate_vertices() {

    std::vector<arma::vec> newVertices;

    const std::vector<arma::vec>& originalVertices = this->originalMesh.get_vertices();

    for(const unsigned int& index: this->subset) {

      newVertices.push_back(originalVertices.at(index));

    }

    this->truncatedMesh.set_vertices(newVertices);

  }

};
#endif
