#include <string>
#include <vector>

#include "settings.h"
#include "image/Image.h"
#include "mesh/MeshIO.h"

#include "CreatePointCloud.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image source;

  source.read().from(settings.source);
  source.segment().cascaded_otsu(settings.cascadeAmount);

  source.write().to(settings.output);

  // output point cloud if wanted
  if(settings.createCloud == true) {

    CreatePointCloud cloud(source);
    cloud.set_flip(settings.flip);

    Mesh pointCloud = cloud.create_point_cloud();
    MeshIO::write(pointCloud, settings.outputCloud);

  }

  return 0;

}
