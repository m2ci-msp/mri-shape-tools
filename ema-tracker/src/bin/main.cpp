#include <armadillo>

#include "settings.h"
#include "EmaData.h"
#include "Tracker.h"
#include "Weights.h"
#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "model/ModelReader.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  EmaData input(settings);

  const arma::vec shift = arma::vec({settings.shiftX, settings.shiftY, settings.shiftZ});

  input.set_shift(shift);

  input.set_scale_factor(settings.scaleFactor);

  ModelReader modelReader(settings.model);
  Model model = modelReader.get_model();
  TrackerData trackerData(model, settings);
  trackerData.sourceIds = settings.sourceIds;

  Tracker tracker(trackerData);
  tracker.update().for_source_ids();

  // fix speaker and create PCA model if desired
  if( settings.fixSpeaker ) {
    const arma::vec speakerWeights = Weights::read(settings.fixedSpeakerWeights);
    tracker.update().to_fixed_speaker(speakerWeights);
  }

  for(int i = 0; i < input.get_size(); ++i) {

    // update data for current time frame
    tracker.data().target = input.get_mesh_at(i);
    tracker.data().currentTime = input.get_time_at(i);

    tracker.fitting().fit();
  }

  Json::Value loggedData = tracker.logger().get_logged_data();

  std::ofstream outFile(settings.outputFileName);

  Json::StyledStreamWriter writer;
  writer.write(outFile, loggedData);

  outFile.close();

  return 0;

}
