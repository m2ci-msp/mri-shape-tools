#include "training/SampleFileReader.h"
#include "model/ModelReader.h"

#include "settings.h"
#include "FidelityMeasurer.h"

int main(int argc, char* argv[]){

  Settings settings(argc, argv);

  ModelReader reader(settings.model);
  Model model = reader.get_model();

  SampleDataBase database = SampleFileReader::read_from(settings.meshes);

  FidelityMeasurer fidelityMeasurer(model, database);
  fidelityMeasurer.set_energy_settings(settings.energySettings) \
    .set_minimizer_settings(settings.minimizerSettings);

  double mean = 0;
  double standardDeviation = 0;

  fidelityMeasurer.measure(
    mean,
    standardDeviation
    );

  std::ofstream outFile(settings.output);

  outFile
    << "{ \"mean\": " << mean << ", "
    << "\"standardDeviation\": " << standardDeviation
    << " }" << std::endl;

  outFile.close();

  return 0;

}
