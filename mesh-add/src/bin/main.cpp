#include "settings.h"
#include "mesh/MeshIO.h"
#include "mesh/MeshCombiner.h"
#include "mesh/NormalEstimation.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  auto first = MeshIO::read(settings.first);
  auto second = MeshIO::read(settings.second);

  if( settings.computeNormals && first.has_faces() ) {

    NormalEstimation estimate(first);

    first.set_vertex_normals( estimate.compute());
  }

  if( settings.computeNormals && second.has_faces() ) {

    NormalEstimation estimate(second);

    second.set_vertex_normals( estimate.compute());
  }

  auto combined = MeshCombiner::combine(first, second);

  if( settings.onlyPoints ) {
    combined.get_faces().clear();
  }

  MeshIO::write(combined, settings.output);

  return 0;

}
