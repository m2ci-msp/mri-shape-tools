#include <fstream>
#include <json/json.h>

#include "energy/Energy.h"
#include "energy/EnergyMinimizer.h"
#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image templateImage, target;
  templateImage.read().from(settings.source);
  target.read().from(settings.target);

  lucasKanade::EnergyData energyData(templateImage, target, settings.center, settings.radius);

  lucasKanade::Energy energy(energyData, settings.energySettings);

  lucasKanade::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

  minimizer.minimize();

  Json::Value root(Json::objectValue);

  root["tx"] = Json::Value(- energy.derived_data().transformation[lucasKanade::TX] * templateImage.info().get_hx());
  root["ty"] = Json::Value(- energy.derived_data().transformation[lucasKanade::TY] * templateImage.info().get_hy());
  root["tz"] = Json::Value(- energy.derived_data().transformation[lucasKanade::TZ] * templateImage.info().get_hz());

  root["alpha"] = Json::Value( -energy.derived_data().transformation[lucasKanade::ALPHA] );
  root["beta"] = Json::Value( -energy.derived_data().transformation[lucasKanade::BETA] );
  root["gamma"] = Json::Value( -energy.derived_data().transformation[lucasKanade::GAMMA] );

  root["ox"] = Json::Value( settings.center(0) * templateImage.info().get_hx());
  root["oy"] = Json::Value( settings.center(1) * templateImage.info().get_hy());
  root["oz"] = Json::Value( settings.center(2) * templateImage.info().get_hz());

  std::ofstream outFile(settings.output);

  Json::StyledStreamWriter writer;
  writer.write(outFile, root);

  outFile.close();

  return 0;

}
