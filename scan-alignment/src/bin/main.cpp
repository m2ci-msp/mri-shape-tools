#include <string>
#include <vector>

#include "settings.h"
#include "mesh/MeshIO.h"
#include "mesh/ProfileMesh.h"
#include "image/Image.h"

#include "EnergyData.h"
#include "Energy.h"
#include "EnergyMinimizer.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh sourceMesh = MeshIO::read(settings.sourceMesh);

  Image sourceScan;
  Image targetScan;

  sourceScan.read().from(settings.sourceScan);
  targetScan.read().from(settings.targetScan);

  ProfileMesh sourceProfileMesh(sourceMesh, sourceScan);

  sourceProfileMesh\
    .set_length(settings.energySettings.profileLength) \
    .set_offset(settings.energySettings.profileOffset);

  sourceProfileMesh.compute_profiles();

  std::vector<arma::vec> sourceProfiles = sourceProfileMesh.get_profiles();

  EnergyData energyData(sourceMesh, sourceProfiles, targetScan);

  Energy energy(energyData, settings.energySettings);

  EnergyMinimizer minimizer(energy, settings.minimizerSettings);

  minimizer.minimize();

  RigidTransformation transformation;

  transformation.set_origin(sourceMesh.get_center());

  transformation.set_translation(
    energyData.transformation[0],
    energyData.transformation[1],
    energyData.transformation[2]
    );

  transformation.set_rotation(
    energyData.transformation[3],
    energyData.transformation[4],
    energyData.transformation[5]
    );


  if(settings.outputMinimizer == true) {

    // try to open file
    std::ofstream outFile(settings.minimizerOutputFile);

    Json::Value output(Json::objectValue);

    Json::Value transformation(Json::objectValue);

    transformation["translationX"] = energyData.transformation[0];
    transformation["translationY"] = energyData.transformation[1];
    transformation["translationZ"] = energyData.transformation[2];

    transformation["rotationX"] = energyData.transformation[3];
    transformation["rotationY"] = energyData.transformation[4];
    transformation["rotationZ"] = energyData.transformation[5];

    output["transformation"] = transformation;

    const int vertexAmount = sourceMesh.get_vertices().size();
    output["correlation"] =  energy.info().energyValue / vertexAmount * (-100);

    Json::StyledStreamWriter writer;
    writer.write(outFile, output);

    outFile.close();

  }

  transformation.apply(sourceMesh);
  MeshIO::write(sourceMesh, settings.output);

  return 0;

}
