#include "mesh/MeshIO.h"

#include "ema/Ema.h"
#include "ema-modify/ApplyModifications.h"

#include "PalateContour.h"
#include "visualize/ProjectMesh.h"
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

  if(settings.outputImage == true) {

    arma::vec min, max;

    result.get_bounding_box(min, max) ;

    const int originX = min(0);
    const int originY = min(1);
    const int originZ = min(2);

    Image image;
    image.create()
      .with_dimension(max(0) - min(0), max(1) - min(1), max(2) - min(2))
      .with_grid_spacing(1, 1, 1)
      .with_origin(originX, originY, originZ)
      .empty_image();

    ProjectMesh projection(result, image, 5);

    image = projection.get_projected_mesh();

    image.write().to(settings.imageOutput);

  }

  return 0;

}
