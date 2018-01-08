#include <string>
#include <vector>

#include "settings.h"
#include "image/Image.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image input;
  input.read().from(settings.inputScan);

  input.crop().to(
      settings.min.at(0),
      settings.min.at(1),
      settings.min.at(2),
      settings.max.at(0),
      settings.max.at(1),
      settings.max.at(2)
      );

  input.write().to(settings.outputScan);

  return 0;

}
