#include "settings.h"
#include "mesh/MeshIO.h"
#include "alignment/RigidAlignment.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh source = MeshIO::read(settings.source);
  Mesh target = MeshIO::read(settings.target);

  RigidAlignment alignment(source, target);

  alignment.set_translation(!settings.noTranslation);
  alignment.set_rotation(!settings.noRotation);
  alignment.set_scaling(!settings.noScaling);

  Mesh changed;

  // decide which mesh to change
  if( settings.changePresent == true ) {
    changed = MeshIO::read(settings.change);
  }
  else {
    changed = source;
  }

  // find transformation and apply to mesh
  alignment.get_transformation().apply(changed);

  MeshIO::write(changed, settings.output);

  return 0;

}
