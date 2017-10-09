#include "mesh/MeshIO.h"
#include "utility/AxisAccess.h"

#include "settings.h"

#include "InterpolatorFactory.h"
#include "MeshBuilder.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh input = MeshIO::read(settings.input);

  AxisAccess access(settings.axisAccess);

  std::vector<arma::vec> controlPoints;
  std::vector<double> targetValues;

  for(const arma::vec& v: input.get_vertices()) {

    controlPoints.push_back({v( access.x() ), v( access.y() )});
    targetValues.push_back(v( access.z() ));

  }

  MeshBuilder builder;

  builder
    .set_control_points(controlPoints)
    .set_sample_amount_x(settings.samplesX)
    .set_sample_amount_y(settings.samplesY);

  // generated mesh has XYZ access
  Mesh result = builder.create();

  InterpolatorFactory factory;

  factory
    .set_control_points(controlPoints)
    .set_target_values(targetValues)
    .set_lambda(settings.lambda);

  Interpolator interpolator = factory.create();

  for(arma::vec& vertex: result.get_vertices()) {

    vertex(2) = interpolator.estimate(vertex(0), vertex(1));

  }

  MeshIO::write(result, settings.output);

}
