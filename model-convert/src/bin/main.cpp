#include "settings.h"
#include "model/ModelReader.h"
#include "FacePageExporter.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  ModelReader reader(settings.model);

  Model model = reader.get_model();

  FacePageExporter exporter(model);
  exporter.write(settings.outputBaseName);

  return 0;

}
