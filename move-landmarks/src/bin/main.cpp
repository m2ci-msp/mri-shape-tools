#include "settings.h"
#include "mesh/MeshIO.h"
#include "landmark/LandmarkIO.h"
#include "alignment/RigidAlignment.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh source = MeshIO::read(settings.source);
  Mesh target = MeshIO::read(settings.target);

  auto landmarks = LandmarkIO::read(settings.landmarks);

  RigidAlignment alignment(source, target);

  alignment.set_translation(!settings.noTranslation);
  alignment.set_rotation(!settings.noRotation);
  alignment.set_scaling(!settings.noScaling);

  // find transformation and apply to landmarks
  alignment.get_transformation().apply(landmarks);

  LandmarkIO::write(landmarks, settings.output);

  return 0;

}
