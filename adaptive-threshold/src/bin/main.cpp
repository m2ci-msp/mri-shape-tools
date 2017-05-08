#include <string>
#include <vector>

#include "settings.h"
#include "scan/ScanIO.h"
#include "mesh/MeshIO.h"

#include "EnergyFunction.h"

#include "CreatePointCloud.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  auto source = ScanIO::read_from(settings.source);

  source.transform()->scale_values(0, 255);
  source.transform()->round();

  EnergyFunction function(source);

  double minEnergy = DBL_MAX;
  double bestThreshold = 0;

  for(int t = 1; t < 255; ++t) {

    const double currentEnergy = function.get_energy(t);
    if( currentEnergy < minEnergy) {
      minEnergy = currentEnergy;
      bestThreshold = t;
    }


  }

  std::cout << bestThreshold << std::endl;
  std::cout << minEnergy << std::endl;

  source.transform()->threshold(bestThreshold);

  ScanIO::write_to(settings.output, source);

  // output point cloud if wanted
  if(settings.createCloud == true) {

    CreatePointCloud cloud(source);
    cloud.set_flip(settings.flip);

    Mesh pointCloud = cloud.create_point_cloud();
    MeshIO::write(pointCloud, settings.outputCloud);

  }

  return 0;

}
