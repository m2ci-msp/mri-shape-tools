#include "settings.h"
#include "WeightsToEma.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  WeightsToEma converter(settings);

  converter.convert_and_save();

  return 0;

}
