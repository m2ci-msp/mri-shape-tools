#ifndef __PROCRUSTES_ALIGNMENT_H__
#define __PROCRUSTES_ALIGNMENT_H__

#include <vector>

#include "mesh/Mesh.h"
#include "mesh/MeshAverager.h"
#include "alignment/RigidAlignment.h"

class ProcrustesAlignment {

public:

  /*--------------------------------------------------------------------------*/

  ProcrustesAlignment(
    const RigidAlignment alignment = RigidAlignment() ) :
    alignment(alignment) {

  }

  /*--------------------------------------------------------------------------*/

  std::vector<Mesh> align_meshes(
    const std::vector<Mesh>& input,
    int iter = 1
    ) {

    this->currentMeshes = input;

    for(int i = 0; i < iter; ++i) {
      perform_procrustes();
    }

    return this->currentMeshes;

  }

  /*--------------------------------------------------------------------------*/

private:

  /*--------------------------------------------------------------------------*/

  void perform_procrustes() {
    compute_mean();
    align_meshes_to_mean();
  }

  /*--------------------------------------------------------------------------*/

  void compute_mean() {
    this->currentMean = MeshAverager::average(this->currentMeshes);
  }

  /*--------------------------------------------------------------------------*/

  void align_meshes_to_mean() {

    this->alignment.set_target(this->currentMean);

    for(Mesh& mesh: this->currentMeshes) {
      this->alignment.set_source(mesh);
      this->alignment.get_transformation().apply(mesh);
    }

  }

  /*--------------------------------------------------------------------------*/

  RigidAlignment alignment;
  std::vector<Mesh> currentMeshes;
  Mesh currentMean;

};

#endif
