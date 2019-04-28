#include "mesh/Mesh.h"
#include "mesh/MeshIO.h"
#include "ExportMeanPosition.h"
#include "ExportBoundingBox.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh mesh = MeshIO::read(settings.input);

  if( settings.exportInfo == "meanPosition" ) {

    ExportMeanPosition(mesh).to(settings.output);

  }

  else if( settings.exportInfo == "boundingBox") {

    ExportBoundingBox(mesh).to(settings.output);

  }

  return 0;

}
