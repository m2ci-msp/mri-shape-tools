#include <string>
#include <vector>

#include "settings.h"
#include "scan/ScanIO.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  auto input = ScanIO::read_from(settings.inputScan);

  input.transform()->crop(
      settings.min.at(0),
      settings.min.at(1),
      settings.min.at(2),
      settings.max.at(0),
      settings.max.at(1),
      settings.max.at(2)
      );

  ScanIO::write_to(settings.outputScan, input);

  return 0;

}
