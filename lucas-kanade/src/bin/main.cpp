#include <fstream>
#include <json/json.h>

#include "mesh/Mesh.h"
#include "mesh/MeshIO.h"

#include "energy/Energy.h"
#include "energy/EnergyMinimizer.h"
#include "settings.h"

#include "energy/BuildTemplateFromMeshProfiles.h"
#include "energy/OriginalTemplate.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image templateImage, target;

  Mesh mesh = MeshIO::read(settings.mesh);

  const double offset = settings.offset;
  const double profileLength = settings.profileLength;

  templateImage.read().from(settings.source);
  target.read().from(settings.target);

  lucasKanade::BuildTemplateFromMeshProfiles builder(mesh, templateImage, offset, profileLength);

  const arma::vec& center = builder.get_center();
  const std::vector<arma::vec>& originalLocations = builder.get_locations();

  const std::vector<double> originalNormalizedValues =
    lucasKanade::OriginalTemplate(originalLocations, templateImage).get_normalized_values();

  lucasKanade::EnergyData energyData(center, originalNormalizedValues, originalLocations, target);

  lucasKanade::Energy energy(energyData, settings.energySettings);

  lucasKanade::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

  minimizer.minimize();

  Json::Value root(Json::objectValue);

  root["tx"] = Json::Value(- energy.data().transformation[lucasKanade::TX] * templateImage.info().get_hx());
  root["ty"] = Json::Value(- energy.data().transformation[lucasKanade::TY] * templateImage.info().get_hy());
  root["tz"] = Json::Value(- energy.data().transformation[lucasKanade::TZ] * templateImage.info().get_hz());

  root["alpha"] = Json::Value( -energy.data().transformation[lucasKanade::ALPHA] );
  root["beta"] = Json::Value( -energy.data().transformation[lucasKanade::BETA] );
  root["gamma"] = Json::Value( -energy.data().transformation[lucasKanade::GAMMA] );

  root["ox"] = Json::Value( center(0) * templateImage.info().get_hx());
  root["oy"] = Json::Value( center(1) * templateImage.info().get_hy());
  root["oz"] = Json::Value( center(2) * templateImage.info().get_hz());

  std::ofstream outFile(settings.output);

  Json::StyledStreamWriter writer;
  writer.write(outFile, root);

  outFile.close();

  return 0;

}
