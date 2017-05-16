#include "settings.h"
#include "Output.h"
#include "ChangeOrigin.h"
#include "mesh/MeshIO.h"

#include "alignment/ProcrustesAlignment.h"
#include "utility/AxisConverter.h"

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

  // change origin if wanted
  if( settings.changeOrigin == true) {

    ChangeOrigin changeOrigin(aligned);
    aligned = changeOrigin.to(settings.originIndex);

  }

  // apply coordinate system mapping
  arma::vec factors{
    settings.xFactor,
    settings.yFactor,
    settings.zFactor
      };

  AxisConverter converter(
    settings.mapping,
    factors
    );

  for(Mesh& mesh: aligned) {
    mesh = converter.convert(mesh);
  }

  Output output(aligned, settings.output);
  output.write();

  return 0;

}
