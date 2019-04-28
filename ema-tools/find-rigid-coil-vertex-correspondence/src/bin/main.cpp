#include "mesh/MeshIO.h"
#include "ema/Ema.h"

#include "settings.h"
#include "CorrespondenceSubsetFinder.h"
#include "CorrespondenceSubsetFinderBuilder.h"
#include "VertexSubsets.h"
#include "OutputResult.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Mesh mesh = MeshIO::read(settings.mesh);

  Ema ema;

  ema.read().from(settings.emaFile);
  ema.reduce_coil_set().to(settings.channels);
  ema.transform_all_coils().scale(settings.scaleFactor);

  std::vector<arma::vec> emaData;

  for(const std::string& coilName: settings.channels) {

    ema.coil(coilName).boundary().change_size(1);
    ema.coil(coilName).mirror().positions();
    emaData.push_back( ema.coil(coilName).interpolate().position_at_time(settings.timeStamp));

  }

  // read subsets
  auto subset = VertexSubsets::read_from(settings.subsets);

  // find correspondences and record energy
  std::vector<unsigned int> vertexIndices;
  double energy;

  CorrespondenceSubsetFinderBuilder() \
    .set_ema_data(emaData) \
    .set_energy_settings(settings.energySettings) \
    .set_minimizer_settings(settings.minimizerSettings) \
    .set_mesh(mesh) \
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
