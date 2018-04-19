#include <fstream>
#include <json/json.h>

#include "mesh/Mesh.h"
#include "mesh/MeshIO.h"

#include "energy/Energy.h"
#include "energy/EnergyMinimizer.h"
#include "settings.h"

#include "OriginalTemplate.h"

#include "LandmarkReader.h"
#include "BuildBoundingBox.h"
#include "BuildTemplateFromBoundingBox.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image templateImage, target;

  templateImage.read().from(settings.source);
  target.read().from(settings.target);

  // read landmakrs
  const std::vector<arma::vec> marks = LandmarkReader(settings.landmarkFile, templateImage).get();

  // build bounding box
  arma::vec min, max;
  BuildBoundingBox(marks).get(min, max);

  // compute locations inside bounding box and center of box
  lucasKanade::BuildTemplateFromBoundingBox builder(min, max, templateImage);

  std::vector<arma::vec> originalLocations = builder.get_locations();
  arma::vec center = builder.get_center();

  lucasKanade::EnergyData energyData(center, originalLocations, templateImage, target);

  lucasKanade::Energy energy(energyData, settings.energySettings);

  lucasKanade::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

  minimizer.minimize();

  Json::Value root(Json::objectValue);

  root["tx"] = Json::Value( energy.data().transformation[lucasKanade::TX]);
  root["ty"] = Json::Value( energy.data().transformation[lucasKanade::TY]);
  root["tz"] = Json::Value( energy.data().transformation[lucasKanade::TZ]);

  root["alpha"] = Json::Value( energy.data().transformation[lucasKanade::ALPHA] );
  root["beta"] = Json::Value( energy.data().transformation[lucasKanade::BETA] );
  root["gamma"] = Json::Value( energy.data().transformation[lucasKanade::GAMMA] );

  root["ox"] = Json::Value( center(0));
  root["oy"] = Json::Value( center(1));
  root["oz"] = Json::Value( center(2));

  root["correlation"] = Json::Value( energy.state().energy * - 100 );

  std::ofstream outFile(settings.output);

  outFile << root << std::endl;

  outFile.close();

  return 0;

}
