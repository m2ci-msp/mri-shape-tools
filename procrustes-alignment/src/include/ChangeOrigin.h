#ifndef __CHANGE_ORIGIN_H__
#define __CHANGE_ORIGIN_H__

#include <vector>

#include "mesh/Mesh.h"
#include "alignment/RigidTransformation.h"

class ChangeOrigin{

public:

  /*--------------------------------------------------------------------------*/

  ChangeOrigin(const std::vector<Mesh>& meshes) : meshes(meshes) {
  }

  /*--------------------------------------------------------------------------*/

  std::vector<Mesh> to(const int& index) {

    std::vector<Mesh> result = this->meshes;

    for(Mesh& mesh: result) {
      const arma::vec frontCenter = mesh.get_vertices().at(index);
      RigidTransformation translation;
      translation.set_translation(-frontCenter);
      translation.apply(mesh);
    }

    return result;

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  std::vector<Mesh> meshes;

  /*--------------------------------------------------------------------------*/

};

#endif
