#include "settings.h"
#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "ema/Ema.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema ema;
  ema.read().from(settings.input);

  EmaCoordinateSystem system;

  system.read_from(settings.system);

  ema.transform_all_coils().map_to_coordinate_system(system);

  ema.write().to(settings.output);

  return 0;

}
