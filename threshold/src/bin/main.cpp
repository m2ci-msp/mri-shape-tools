#include "image/Image.h"
#include "utility/CreatePointCloud.h"
#include "mesh/MeshIO.h"
#include "image/filter/diffusion/DiffusionSettings.h"

#include "settings.h"

int main(int argc, char* argv[]) {

    Settings settings(argc, argv);

    Image image;
    image.read().from(settings.input);

    if(settings.scaleTo8Bit == true) {

      image.values().scale(0, 255);
      image.values().round();

    }

    image.segment().threshold(settings.threshold);

    image.write().to(settings.output);

    // output point cloud if wanted
    if(settings.createCloud == true) {

      DiffusionSettings diffusionSettings;
      diffusionSettings.presmoothSigma = settings.presmoothSigma;
      diffusionSettings.integrationRho = settings.integrationRho;

      CreatePointCloud cloud(image.data(), diffusionSettings);
      cloud.set_flip(settings.flip);

      Mesh pointCloud = cloud.create_point_cloud();
      MeshIO::write(pointCloud, settings.outputCloud);

    }

    return 0;

}
