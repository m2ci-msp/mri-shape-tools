#include <string>
#include <vector>

#include "settings.h"
#include "image/Image.h"
#include "mesh/MeshIO.h"

#include "CreatePointCloud.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image input;

  input.read().from(settings.input);
  input.segment().otsu();

  input.write().to(settings.output);

  // output point cloud if wanted
  if(settings.createCloud == true) {

    CreatePointCloud cloud(input);
    cloud.set_flip(settings.flip);

    Mesh pointCloud = cloud.create_point_cloud();
    MeshIO::write(pointCloud, settings.outputCloud);

  }

  return 0;

}
