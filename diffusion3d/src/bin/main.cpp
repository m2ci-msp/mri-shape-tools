#include "scan/Scan.h"
#include "scan/ScanIO.h"
#include "diffusion/Diffusion.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Scan scan = ScanIO::read_from(settings.inputScan);
  scan.transform()->scale_values(0, 255);

  const int& nx = scan.data()->get_nx();
  const int& ny = scan.data()->get_ny();
  const int& nz = scan.data()->get_nz();

  ImageData imageData;
  imageData.set_values(scan.data()->get_values(), nx, ny, nz, 0, 0, 0);

  Image image(imageData);

  Diffusion diffusion(image, settings.diffusionSettings);
  image = diffusion.get_result();

  image.data().set_boundary_sizes(0, 0, 0);

  scan.data()->set_data(image.data().values);

  ScanIO::write_to(settings.outputScan, scan);

}
