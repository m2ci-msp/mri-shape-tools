#include <vector>

#include <armadillo>

#include "settings.h"
#include "mesh/MeshIO.h"
#include "model/ModelReader.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  ModelReader reader(settings.model);

  const Model model = reader.get_model();

  const Mesh mesh = model.data().get_shape_space_origin_mesh();
  
  MeshIO::write(mesh, settings.output);

  return 0;

}
