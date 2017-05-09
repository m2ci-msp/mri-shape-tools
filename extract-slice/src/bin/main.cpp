#include <string>
#include <vector>
#include <stdexcept>

#include "settings.h"
#include "ProcessScan.h"
#include "ImageStack.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  ProcessScan process(settings.input, settings.discard);

  auto stack = process.get_image_stack();

  Cairo::RefPtr<Cairo::ImageSurface> image;

  if( (
        settings.xyOutput ||
        settings.xzOutput ||
        settings.yzOutput ) == false ) {

    throw std::runtime_error("No slices given for output.");
  }

  // output xy slices if wanted
  if( settings.xyOutput == true ) {
    for( size_t i = 0; i < settings.xySlices.size(); ++i) {

      const int sliceIndex = settings.xySlices.at(i);

      image = stack->get_xy_slices().at(sliceIndex);

      std::string outputName =
        settings.baseName + "_xy_" + std::to_string(i) + ".png";

      image->write_to_png(outputName);
    }
  }

  // output xz slices if wanted
  if( settings.xzOutput == true ) {
    for( size_t i = 0; i < settings.xzSlices.size(); ++i) {

      const int sliceIndex = settings.xzSlices.at(i);

      image = stack->get_zx_slices().at(sliceIndex);

      std::string outputName =
        settings.baseName + "_xz_" + std::to_string(i) + ".png";

      image->write_to_png(outputName);
    }
  }

  // output yz slices if wanted
  if( settings.yzOutput == true ) {
    for( size_t i = 0; i < settings.yzSlices.size(); ++i) {

      const int sliceIndex = settings.yzSlices.at(i);

      image = stack->get_zy_slices().at(sliceIndex);

      std::string outputName =
        settings.baseName + "_yz_" + std::to_string(i) + ".png";

      image->write_to_png(outputName);
    }
  }


  return 0;

}
