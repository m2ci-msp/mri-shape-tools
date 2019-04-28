#include "settings.h"
#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "ema/Ema.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema ema;
  ema.read().from(settings.input);

  for(const std::string& channelName : settings.channels ) {

    Mesh result = ema.coil(channelName).positions().as_point_cloud();

    MeshIO::write(result, settings.output + "_" + channelName + ".ply");
  }

  return 0;

}
