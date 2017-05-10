#ifndef __MESH_COMBINER_H__
#define __MESH_COMBINER_H__

#include <vector>
#include <armadillo>

#include "mesh/Mesh.h"

class MeshCombiner{

public:

  /*--------------------------------------------------------------------------*/

  static Mesh combine(const Mesh& first, const Mesh& second) {

    Mesh result;

    // combine vertex information
    result.set_vertices(
      combine_vectors(first.get_vertices(), second.get_vertices())
      );

    // combine face information

    // shift indices first
    const int offset = first.get_vertices().size();
    auto secondFaces = second.get_faces();
    shift_face_indices(secondFaces, offset);

    result.set_faces(
      combine_vectors(first.get_faces(), secondFaces)
      );

    // combine normal information if available
    if( first.has_normals() && second.has_normals() ) {
      result.set_vertex_normals(
        combine_vectors(first.get_vertex_normals(), second.get_vertex_normals())
        );
    }

    return result;

  }

private:

  /*--------------------------------------------------------------------------*/

  template<typename T>
  static std::vector<T> combine_vectors(
    const std::vector<T>& first, const std::vector<T>& second
    ) {

    std::vector<T> result(first);

    result.insert(
      std::end(result), std::begin(second),
      std::end(second)
      );

    return result;

  }

  /*--------------------------------------------------------------------------*/

  static void shift_face_indices(
    std::vector< std::vector<unsigned int> >& faces,
    const int& shift) {

    for(auto& face : faces ) {
      for(auto& index: face ) {
        index += shift;
      }
    }
  }

  /*--------------------------------------------------------------------------*/

};
#endif
