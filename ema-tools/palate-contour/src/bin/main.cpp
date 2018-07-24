#include "mesh/MeshIO.h"

#include "ema/Ema.h"
#include "ema-modify/ApplyModifications.h"

#include "PalateContour.h"
#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema ema;
  std::vector<arma::vec> points;

  for(const std::string& fileName: settings.input) {

    ema.read().from(fileName);

    if(settings.applyModifications == true) {

      emaModify::ApplyModifications(ema).apply(settings.emaModifications);

    }

    for(const std::string& channel: settings.channels) {

      ema.coil(channel).transform().scale(settings.scale);

      for(int i = 0; i < ema.info().sample_amount(); ++i) {

        points.push_back(ema.coil(channel).access().position(i));

      }
    }

  }

  PalateContour contour(points, settings.axisAccess);

  Mesh result;

  result.set_vertices(contour.get_boundary(settings.spacing));

  MeshIO::write(result, settings.output);

  return 0;

}
