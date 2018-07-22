#include <armadillo>

#include "ema/Ema.h"
#include "ema-modify/ApplyModifications.h"


#include "settings.h"
#include "Tracker.h"
#include "Weights.h"
#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "model/ModelReader.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema ema;

  ema.read().from(settings.inputFileName);

  // reduce ema data to selected coil subset
  ema.reduce_coil_set().to(settings.channels);

  if(settings.applyModifications == true) {

    emaModify::ApplyModifications(ema).apply(settings.emaModifications);

  }

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

  for(int i = 0; i < ema.info().sample_amount(); ++i) {

    // update data for current time frame
    tracker.data().target = ema.point_cloud().from(settings.channels, i);
    tracker.data().currentTime = ema.info().time_stamps().at(i);

    tracker.fitting().fit();

  }

  Json::Value loggedData = tracker.logger().get_logged_data();

  std::ofstream outFile(settings.outputFileName);

  outFile << loggedData << std::endl;

  outFile.close();

  return 0;

}
