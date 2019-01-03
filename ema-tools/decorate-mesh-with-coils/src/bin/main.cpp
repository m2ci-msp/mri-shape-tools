#include "mesh/Mesh.h"
#include "mesh/MeshIO.h"
#include "mesh/MeshCombiner.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh mesh = MeshIO::read(settings.mainMesh);
  Mesh coil = MeshIO::read(settings.coilMesh);

  const arma::vec center = coil.get_center();

  for(int index: settings.indices) {

    arma::vec newCenter = mesh.get_vertices().at(index);

    Mesh copy = coil;

    for(arma::vec& vertex: copy.get_vertices()) {

      vertex = vertex - center + newCenter;

    }

    mesh = MeshCombiner::combine(mesh, copy);

  }

  MeshIO::write(mesh, settings.output);

  return 0;

}
