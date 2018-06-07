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

  rigidAlignment::EnergyData data(source, target);

  if(settings.landmarksPresent == true) {

    data.landmarks = LandmarkIO::read(settings.landmarks);

  }

  if( settings.initializeWithLandmarks == true ) {

    settings.energySettings.weights["dataTerm"] = 0;
    settings.energySettings.weights["landmarkTerm"] = 1;

    rigidAlignment::Energy energy(data, settings.energySettings);
    rigidAlignment::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

    minimizer.minimize();

    // replace source with aligned one
    data.source = energy.derived_data().source;

  }

  settings.energySettings.weights["dataTerm"] = 1;
  settings.energySettings.weights["landmarkTerm"] = 1;

  rigidAlignment::Energy energy(data, settings.energySettings);

  rigidAlignment::EnergyMinimizer minimizer(energy, settings.minimizerSettings);

  minimizer.minimize();

  MeshIO::write(energy.derived_data().source, settings.output);

  return 0;

}
