#include "settings.h"
#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "ema/Ema.h"
#include "ema/EmaCoordinateSystem.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema ema;
  ema.read().from(settings.input);

  EmaCoordinateSystem system;

  const arma::vec left = ema.coil(settings.leftCoil).access().position(settings.timeFrame);
  const arma::vec right = ema.coil(settings.rightCoil).access().position(settings.timeFrame);
  const arma::vec front = ema.coil(settings.frontCoil).access().position(settings.timeFrame);

  system.build_from(left, right, front);

  system.write_to(settings.output);

  return 0;

}
