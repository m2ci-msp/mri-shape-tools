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

  return 0;

}
