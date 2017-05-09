#include "settings.h"
#include "Output.h"
#include "ChangeOrigin.h"
#include "SuzanneMapping.h"
#include "mesh/MeshIO.h"
#include "alignment/ProcrustesAlignment.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  std::vector<Mesh> meshCollection;

  for(const auto& file: settings.input) {
    meshCollection.push_back(MeshIO::read(file));
  }

  RigidAlignment alignment;

  alignment.set_translation(!settings.noTranslation);
  alignment.set_rotation(!settings.noRotation);
  alignment.set_scaling(!settings.noScaling);

  ProcrustesAlignment procrustes(alignment);

  std::vector<Mesh> aligned =
    procrustes.align_meshes(meshCollection, settings.iter);

  if( settings.changeOrigin == true) {

    ChangeOrigin changeOrigin(aligned);
    aligned = changeOrigin.to(settings.originIndex);

  }

  if( settings.suzanneMapping == true) {

    SuzanneMapping mapping(aligned);
    aligned = mapping.map();

  }

  Output output(aligned, settings.output);
  output.write();

  return 0;

}
