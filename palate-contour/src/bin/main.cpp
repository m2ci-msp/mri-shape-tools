#include "mesh/MeshIO.h"

#include "EmaPoints.h"

#include "PalateContour.h"
#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  EmaPoints points;

  points.set_scale(settings.scale);
  points.set_channels(settings.channels);

  for(const std::string& fileName: settings.input ) {
    points.add_points_from_file(fileName);
  }

  PalateContour contour(points.get_points(), settings.axisAccess);

  Mesh result;

  result.set_vertices(contour.get_boundary(settings.radius, settings.samplesX, settings.samplesY));

  MeshIO::write(result, settings.output);

  return 0;

}
