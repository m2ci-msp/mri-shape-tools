#include <armadillo>

#include "settings.h"

#include "landmark/LandmarkIO.h"

#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"

#include "optimization/rigidalignment/Energy.h"
#include "optimization/rigidalignment/EnergyMinimizer.h"
#include "optimization/rigidalignment/MinimizerSettings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  // read input data
  Mesh source = MeshIO::read(settings.source);
  Mesh target = MeshIO::read(settings.target);

  // deal with target meshes that do not provide normals
  if( target.has_normals() == false ) {

    // try to estimate normals if the mesh has faces
    if( target.has_faces() == true ) {

      NormalEstimation estimation(target);
      target.set_vertex_normals(estimation.compute());

    } // end if
    else {

      // use point-to-point distance measure
      settings.energySettings.searchStrategy =
        rigidAlignment::EnergySettings::SearchStrategy::BASIC;

    } // end else

  } // end if

  std::vector<Landmark> landmarks;

  if(settings.landmarksPresent == true) {

    landmarks = LandmarkIO::read(settings.landmarks);

  }

  // first initialize with landmark information alone
  if( settings.noInitializationWithLandmarks == false && settings.landmarksPresent == true) {

    rigidAlignment::EnergyData data(source, target);
    data.landmarks = landmarks;

    settings.energySettings.weights["dataTerm"] = 0;
    settings.energySettings.weights["landmarkTerm"] = 1;

    // do not optimize rotation: might drastically shrink the template
    const bool noScaling = settings.minimizerSettings.noScaling;

    // only use one iteration: landmarks do not change between iterations
    const int iterationAmount = settings.minimizerSettings.iterationAmount;


    settings.minimizerSettings.noScaling = true;
    settings.minimizerSettings.iterationAmount = 1;

    rigidAlignment::Energy energy(data, settings.energySettings);
    rigidAlignment::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

    minimizer.minimize();

    // replace source with aligned one
    source = energy.derived_data().source;

    // restore old settings
    settings.minimizerSettings.noScaling = noScaling;
    settings.minimizerSettings.iterationAmount = iterationAmount;

  }

  // now use both, data and landmarks
  if( settings.noCombinedOptimization == false && settings.landmarksPresent == true) {

    rigidAlignment::EnergyData data(source, target);

    data.landmarks = landmarks;

    settings.energySettings.weights["dataTerm"] = 1;
    settings.energySettings.weights["landmarkTerm"] = 1;

    rigidAlignment::Energy energy(data, settings.energySettings);
    rigidAlignment::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

    minimizer.minimize();

    // replace source with aligned one
    source = energy.derived_data().source;


  }

  // final step: only use data
  if( settings.noDataOnlyOptimization == false) {

    rigidAlignment::EnergyData data(source, target);

    settings.energySettings.weights["dataTerm"] = 1;
    settings.energySettings.weights["landmarkTerm"] = 0;

    rigidAlignment::Energy energy(data, settings.energySettings);
    rigidAlignment::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

    minimizer.minimize();

    // replace source with aligned one
    source = energy.derived_data().source;

  }

  MeshIO::write(source, settings.output);

  return 0;

}
