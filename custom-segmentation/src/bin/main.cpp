#include "settings.h"
#include "image/Image.h"
#include "image-modify/ApplyModifications.h"
#include "mesh/MeshIO.h"

#include "CreatePointCloud.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image source;

  source.read().from(settings.source);

  ApplyModifications(source).apply(settings.specifications);

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
