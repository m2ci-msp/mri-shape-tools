#include "settings.h"
#include "TransformationWriter.h"
#include "alignment/RigidAlignment.h"

#include "mesh/MeshIO.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh source = MeshIO::read(settings.source);
  Mesh target = MeshIO::read(settings.target);

  RigidAlignment alignment(source, target);

  alignment.set_translation(!settings.noTranslation);
  alignment.set_rotation(!settings.noRotation);
  alignment.set_scaling(!settings.noScaling);

  TransformationWriter::write( alignment.get_transformation(), settings.output );

  return 0;

}
