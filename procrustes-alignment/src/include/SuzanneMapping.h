#ifndef __SUZANNE_MAPPING_H__
#define __SUZANNE_MAPPING_H__

#include <vector>

#include "mesh/Mesh.h"

class SuzanneMapping{

public:

  SuzanneMapping(const std::vector<Mesh>& meshes) :
    meshes(meshes) {
  }

  std::vector<Mesh> map() {

    std::vector<Mesh> result = this->meshes;

    for(Mesh& mesh: result) {

      for(arma::vec& vertex: mesh.get_vertices()) {

        const arma::vec tmp = vertex;

        vertex[0] = -tmp[2];
        vertex[1] =  tmp[0];
        vertex[2] = -tmp[1];

      }

    }

    return result;

  }

  std::vector<Mesh> meshes;

};

#endif
