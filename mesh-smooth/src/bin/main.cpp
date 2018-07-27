#include "settings.h"
#include "mesh/MeshIO.h"
#include "mesh/MeshSmooth.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh source = MeshIO::read(settings.input);

  MeshSmooth(source).apply(settings.iterationAmount);

  MeshIO::write(source, settings.output);

  return 0;

}
