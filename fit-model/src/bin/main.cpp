#include <armadillo>

#include "settings.h"

#include "landmark/LandmarkIO.h"

#include "model/Model.h"
#include "model/ModelReader.h"
#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "mesh/MeshSmooth.h"

#include "optimization/fitmodel/Energy.h"
#include "optimization/fitmodel/EnergyMinimizer.h"
#include "optimization/fitmodel/MinimizerSettings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  // read input data
  Mesh target = MeshIO::read(settings.target);
  ModelReader reader(settings.model);
  Model model = reader.get_model();

  // deal with target meshes that do not provide normals
  if( settings.fixedNeighbors == false && target.has_normals() == false ) {

    // try to estimate normals if the mesh has faces
    if( target.has_faces() == true ) {
      NormalEstimation estimation(target);
      target.set_vertex_normals(estimation.compute());
    } // end if
    else {
      // use point-to-point distance measure
      settings.energySettings.searchStrategy =
        fitModel::EnergySettings::SearchStrategy::BASIC;
    } // end else

  } // end if

  fitModel::EnergyData data(model, target);

  if(settings.landmarksPresent == true) {
    data.landmarks = LandmarkIO::read(settings.landmarks);

    // use landmarks only for initialization if wanted
    if(settings.useLandmarksOnlyForInitialization == true) {

      fitModel::Energy energy(data, settings.energySettings);

      fitModel::MinimizerSettings minimizerSettings = settings.minimizerSettings;

      // use one iteration
      minimizerSettings.iterationAmount = 1;
      fitModel::EnergyMinimizer minimizer(energy, minimizerSettings);

      minimizer.minimize();

      // copy weights
      data.speakerWeights = energy.data().speakerWeights;
      data.phonemeWeights = energy.data().phonemeWeights;

      // remove landmarks
      data.landmarks.clear();

    } // end if useLandmarks
  } // end if landmarksPresent

  // configure for fitting only
  settings.energySettings.weights["dataTerm"] = 1;
  settings.energySettings.weights["landmarkTerm"] = settings.landmarkTermWeight;

  fitModel::Energy energy(data, settings.energySettings);

  fitModel::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

  minimizer.minimize();

  Mesh result = energy.derived_data().source;

  // remove normals
  result.get_vertex_normals().clear();

  MeshSmooth(result).apply(settings.meshSmoothIterations);

  MeshIO::write(result, settings.output);

  return 0;

}
