#include "model/Model.h"
#include "model/ModelReader.h"
#include "ema/Ema.h"
#include "ema-modify/ApplyModifications.h"

#include "settings.h"
#include "CorrespondenceSubsetFinder.h"
#include "CorrespondenceSubsetFinderBuilder.h"
#include "VertexSubsets.h"
#include "OutputResult.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  // read model
  Model model = ModelReader(settings.model).get_model();

  Ema ema;

  ema.read().from(settings.emaFile);
  ema.reduce_coil_set().to(settings.channels);

  if( settings.applyModifications == true) {

    emaModify::ApplyModifications(ema).apply(settings.emaModifications);

  }

  std::vector<arma::vec> emaData =
    ema.point_cloud().from(settings.channels, settings.timeFrame).get_vertices();

  // read subsets
  auto subset = VertexSubsets::read_from(settings.subsets);

  // find correspondences and record energy
  std::vector<unsigned int> vertexIndices;
  double energy;

  CorrespondenceSubsetFinderBuilder() \
    .set_ema_data(emaData) \
    .set_energy_settings(settings.energySettings) \
    .set_minimizer_settings(settings.minimizerSettings) \
    .set_model(model) \
    .set_subsets(subset) \
    .set_partition_index(settings.partitionIndex) \
    .set_partition_amount(settings.partitionAmount) \
    .build() \
    .find_best_correspondence() \
    .get_best_correspondence(vertexIndices) \
    .get_best_energy(energy);

  // output result
  OutputResult() \
    .set_vertex_indices(vertexIndices) \
    .set_ema_channels(settings.channels) \
    .set_energy(energy) \
    .set_partition_amount(settings.partitionAmount) \
    .set_partition_index(settings.partitionIndex) \
    .write(settings.outputFile);

}
