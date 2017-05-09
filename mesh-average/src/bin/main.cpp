#include "settings.h"
#include "mesh/MeshIO.h"
#include "mesh/MeshAverager.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  std::vector<Mesh> meshCollection;

  for(const auto& file: settings.input) {
    meshCollection.push_back(MeshIO::read(file));
  }

  auto averaged = MeshAverager::average(meshCollection);

  MeshIO::write(averaged, settings.output);

  return 0;

}
